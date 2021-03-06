// osgdb_osgearth_bingd.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <osgEarth/TileSource>
#include <osgEarth/Registry>
#include <osgEarth/URI>
#include <osgEarth/StringUtils>
#include <osgEarth/Random>
#include <osgEarth/ImageUtils>
#include <osgEarth/Containers>

#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/GeometryRasterizer>

#include <osgDB/FileNameUtils>
#include <osgText/Font>

#include <OpenThreads/Atomic>
#include <osgEarth/SpatialReference>
#include "osgDB/WriteFile"
#include "osgDB/ReadFile"
#include <stdio.h>
#include <io.h>
#include <direct.h> //用 _mkdir()创建一个文件夹（目录）
#include "osgDB/FileUtils"
#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")

using namespace osgEarth;
using namespace osgEarth::Symbology;


std::string g_appRunDir = "";

#define LC "[Bing] "


namespace osgEarth {
	namespace Drivers
	{
		using namespace osgEarth;

		/**
		* Configuration structure for the Microsoft Bing driver.
		* http://www.bing.com/developers/
		*
		* Using Bing requires an API key. You can get one from the URL above.
		* You are responsible for complying with the Bing terms of service.
		*/
		class GooglemapOptions : public TileSourceOptions // NO EXPORT; header only
		{
		public:

			/**
			* API Key to use to access Bing REST services. Required!
			*/
			optional<std::string>& key() { return _apiKey; }
			const optional<std::string>& key() const { return _apiKey; }

			/**
			* Imagery set to access. Default is "Aerial".
			*
			* As of this writing, options are:
			*    Aerial
			*    AerialWithImagery
			*    Road
			*
			* (The "Birdseye" layers don't work with this driver at this time.)
			*
			* See (http://msdn.microsoft.com/en-us/library/ff701716.aspx) for more information.
			*/
			optional<std::string>& imagerySet() { return _imagerySet; }
			const optional<std::string>& imagerySet() const { return _imagerySet; }

			/**
			* Base URL for the Bing REST API. By default this will point to the
			* Internet Bing services.
			*/
			optional<std::string>& imageryMetadataAPI() { return _imageryMetadataAPI; }
			const optional<std::string>& imageryMetadataAPI() const { return _imageryMetadataAPI; }


		public:
			/**
			* Constructs a Bing configuration structure.
			*/
			GooglemapOptions(const TileSourceOptions& opt = TileSourceOptions()) : TileSourceOptions(opt),
				_imagerySet("Aerial"),
				_imageryMetadataAPI("http://dev.virtualearth.net/REST/v1/Imagery/Metadata")
			{
				setDriver("googlemap");
				fromConfig(_conf);
			}

			/** dtor */
			virtual ~GooglemapOptions() { }

		public:
			Config getConfig() const {
				Config conf = TileSourceOptions::getConfig();
				conf.updateIfSet("key", _apiKey);
				conf.updateIfSet("imagery_set", _imagerySet);
				conf.updateIfSet("imagery_metadata_api", _imageryMetadataAPI);
				return conf;
			}

		protected:
			void mergeConfig(const Config& conf) {
				TileSourceOptions::mergeConfig(conf);
				fromConfig(conf);
			}

		private:
			void fromConfig(const Config& conf) {
				conf.getIfSet("key", _apiKey);
				conf.getIfSet("imagery_set", _imagerySet);
				conf.getIfSet("imagery_metadata_api", _imageryMetadataAPI);
			}

			optional<std::string> _apiKey;
			optional<std::string> _imagerySet;
			optional<std::string> _imageryMetadataAPI;
		};

	}
} // namespace osgEarth::Drivers

namespace
{
	struct AlphaBlend
	{
		bool operator()(const osg::Vec4f& src, osg::Vec4f& dest)
		{
			float sa = src.a();
			dest.set(
				dest.r()*(1.0f - sa) + src.r()*sa,
				dest.g()*(1.0f - sa) + src.g()*sa,
				dest.b()*(1.0f - sa) + src.b()*sa,
				dest.a());
			return true;
		}
	};

	typedef LRUCache<std::string, std::string> TileURICache;
}

#include <osgEarth/TileSource>



class BingTileSource : public TileSource
{
private:
	osgEarth::Drivers::GooglemapOptions _options;
	osg::ref_ptr<osgDB::Options>   _dbOptions;
	Random                         _prng;
	bool                           _debugDirect;
	osg::ref_ptr<Geometry>         _geom;
	osg::ref_ptr<osgText::Font>    _font;
	TileURICache                   _tileURICache;
	OpenThreads::Atomic            _apiCount;

	std::vector<std::string> _urls;
	bool mIsConnected;

public:
	/**
	* Constructs the tile source
	*/
	BingTileSource(const TileSourceOptions& options) :
		TileSource(options),
		_options(options),
		_debugDirect(true),
		_tileURICache(true, 1024u)
	{
		if (::getenv("OSGEARTH_BING_DIRECT"))
			_debugDirect = true;

		if (::getenv("OSGEARTH_BING_DEBUG"))
		{
			_geom = new Ring();
			_geom->push_back(osg::Vec3(10, 10, 0));
			_geom->push_back(osg::Vec3(245, 10, 0));
			_geom->push_back(osg::Vec3(245, 245, 0));
			_geom->push_back(osg::Vec3(10, 245, 0));
			_font = Registry::instance()->getDefaultFont();
		}
		mIsConnected = false;
	}

	/**
	* One-tile tile source initialization.
	*/
	Status initialize(const osgDB::Options* dbOptions)
	{
		// Always apply the NO CACHE policy.
		_dbOptions = Registry::instance()->cloneOrCreateOptions(dbOptions);
		//CachePolicy::NO_CACHE.apply( _dbOptions.get() );

		// If the user did not include an API key, fail.
		/*if ( !_options.key().isSet() )
		{
		return Status::Error("Bing API key is required");
		}*/

		// If the user did not specify an imagery set, default to aerial.
		if (!_options.imagerySet().isSet())
		{
			_options.imagerySet() = "Aerial";
		}

		// Bing maps profile is spherical mercator with 2x2 tiles are the root.
		/*const Profile* profile = Profile::create(
		SpatialReference::get("spherical-mercator"),
		MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
		2, 2);*/

		SpatialReference* spatialReference = SpatialReference::get("EPSG:4326");
#if _arcgis_
		const Profile* profile = Profile::create(
			spatialReference,
			-180, -90, 180, 90,
			2, 1);
#else
		//arcgis_imagery_world
		/*const Profile* profile = Profile::create(
		spatialReference,
		-180, -90, 180, 90,
		1, 1);*/

		//gaode_imagery_world
		/*const Profile* profile = Profile::create(
		SpatialReference::get("spherical-mercator"),
		MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
		1, 1);*/

		//tianditu_imagery_world
		/*const Profile* profile = Profile::create(
		spatialReference,
		-180, -90, 180, 90,
		2, 1);*/

		//google image tile
		const Profile* profile = Profile::create(
			SpatialReference::get("spherical-mercator"),
			MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
			1, 1);
#endif

		setProfile(profile);
		DWORD flag;
		BOOL con = InternetGetConnectedState(&flag, 0);
		this->mIsConnected = con;
		return STATUS_OK;
	}

	/**
	* Tell the terrain engine not to cache tiles form this source.
	*/
	CachePolicy getCachePolicyHint(const Profile*) const
	{
		return CachePolicy::DEFAULT;
	}


	/**
	* Create and return an image for the given TileKey.
	*/
	osg::Image* createImage(const TileKey& key, ProgressCallback* progress)
	{
		if (!this->mIsConnected)
			return NULL;
		std::string url = this->getDirectURI(key);
		osg::Image* pResult = URI(url).getImage(_dbOptions.get(), progress);
		return pResult;
		if (_debugDirect)
		{
			if (g_appRunDir == "")
			{
				std::string path = osgDB::getCurrentWorkingDirectory();
				g_appRunDir = path;
			}
			
			std::stringstream cachePath;
			cachePath << g_appRunDir;
			cachePath << "\\osgearth_cache" << "\\googleMap" << "\\" 
				<< key.getLOD() << "\\" << key.getTileX() << "\\" << key.getTileY() << ".jpg";
			osg::Image* pImage = NULL;
			if (osgDB::fileExists(cachePath.str()))
				pImage = osgDB::readImageFile(cachePath.str());
			if (pImage != NULL)
			{
				return pImage;
			}
			else
			{
				/*std::string curRunPath = g_appRunDir;
				curRunPath += "\\osgearth_cache";
				if (!osgDB::fileExists(curRunPath))
				{
					osgDB::makeDirectory(curRunPath);
				}
				curRunPath += "\\googleMap";
				if (!osgDB::fileExists(curRunPath))
				{
					osgDB::makeDirectory(curRunPath);
				}
				std::stringstream level;
				level << key.getLOD();
				curRunPath += ("\\" + level.str());
				if (!osgDB::fileExists(curRunPath))
				{
					osgDB::makeDirectory(curRunPath);
				}

				std::stringstream row;
				row << key.getTileX();
				curRunPath += ("\\" + row.str());
				if (!osgDB::fileExists(curRunPath))
				{
					osgDB::makeDirectory(curRunPath);
				}*/

				std::string url = this->getDirectURI(key);
				pImage = URI(url).getImage(_dbOptions.get(), progress);
				/*if (pImage != NULL)
				{
					bool r = osgDB::writeImageFile(*pImage, cachePath.str());
					if (r)
					{
						r = 0;
					}
				}*/
				return pImage;
			}
		}

		// center point of the tile (will be in spherical mercator)
		double x, y;
		key.getExtent().getCentroid(x, y);

		// transform it to lat/long:
		GeoPoint geo;

		GeoPoint(getProfile()->getSRS(), x, y).transform(
			getProfile()->getSRS()->getGeographicSRS(),
			geo);

		// contact the REST API. Docs are here:
		// http://msdn.microsoft.com/en-us/library/ff701716.aspx

		// construct the request URI:
		std::string request = Stringify()
			<< std::setprecision(12)
			<< _options.imageryMetadataAPI().get()     // base REST API
			<< "/" << _options.imagerySet().get()   // imagery set to use
			<< "/" << geo.y() << "," << geo.x()     // center point in lat/long
			<< "?zl=" << key.getLOD() + 1              // zoom level
			<< "&o=json"                               // response format
			<< "&key=" << _options.key().get();        // API key

		// check the URI cache.
		URI                  location;
		TileURICache::Record rec;

		if (_tileURICache.get(request, rec))
		{
			location = URI(rec.value());
			//CacheStats stats = _tileURICache.getStats();
			//OE_INFO << "Ratio = " << (stats._hitRatio*100) << "%" << std::endl;
		}
		else
		{
			unsigned c = ++_apiCount;
			if (c % 25 == 0)
				OE_INFO << LC << "API calls = " << c << std::endl;

			// fetch it:
			ReadResult metadataResult = URI(request).readString(_dbOptions, progress);

			if (metadataResult.failed())
			{
				// check for a REST error:
				if (metadataResult.code() == ReadResult::RESULT_SERVER_ERROR)
				{
					OE_WARN << LC << "REST API request error!" << std::endl;

					Config metadata;
					std::string content = metadataResult.getString();
					metadata.fromJSON(content);
					ConfigSet errors = metadata.child("errorDetails").children();
					for (ConfigSet::const_iterator i = errors.begin(); i != errors.end(); ++i)
					{
						OE_WARN << LC << "REST API: " << i->value() << std::endl;
					}
					return 0L;
				}
				else
				{
					OE_WARN << LC << "Request error: " << metadataResult.getResultCodeString() << std::endl;
				}
				return 0L;
			}

			// decode it:
			Config metadata;
			if (!metadata.fromJSON(metadataResult.getString()))
			{
				OE_WARN << LC << "Error decoding REST API response" << std::endl;
				return 0L;
			}

			// check the vintage field. If it's empty, that means we got a "no data" tile.
			Config* vintageEnd = metadata.find("vintageEnd");
			if (!vintageEnd || vintageEnd->value().empty())
			{
				OE_DEBUG << LC << "NO data image encountered." << std::endl;
				return 0L;
			}

			// find the tile URI:
			Config* locationConf = metadata.find("imageUrl");
			if (!locationConf)
			{
				OE_WARN << LC << "REST API JSON parsing error (imageUrl not found)" << std::endl;
				return 0L;
			}

			location = URI(locationConf->value());
			_tileURICache.insert(request, location.full());
		}

		// request the actual tile
		//OE_INFO << "key = " << key.str() << ", URL = " << location->value() << std::endl;

		//osg::Image* image = location.getImage(_dbOptions.get(), progress);
		osg::Image* image = osgDB::readImageFile(location.full());
		/*****************************************************************************************************************/
		/*if ( image &&  _geom.valid() )
		{
		GeometryRasterizer rasterizer( image->s(), image->t() );
		rasterizer.draw( _geom.get(), osg::Vec4(1,1,1,1) );
		osg::ref_ptr<osg::Image> overlay = rasterizer.finalize();
		ImageUtils::PixelVisitor<AlphaBlend> blend;
		blend.accept( overlay.get(), image );
		}*/

		return image;
	}

private:

	std::string getQuadKey(const TileKey& key)
	{
		unsigned int tile_x, tile_y;
		key.getTileXY(tile_x, tile_y);
		unsigned int lod = key.getLevelOfDetail();

		std::stringstream ss;
		for (unsigned i = (int)lod + 1; i > 0; i--)
		{
			char digit = '0';
			unsigned mask = 1 << (i - 1);
			if ((tile_x & mask) != 0)
			{
				digit++;
			}
			if ((tile_y & mask) != 0)
			{
				digit += 2;
			}
			ss << digit;
		}
		OE_WARN << LC << "ss: " << ss.str() << std::endl;
		return ss.str();
	}
	std::string getLodKey(const TileKey& key)
	{
		unsigned int lod = key.getLevelOfDetail() + 2;
		std::stringstream sLod;
		if (lod<10)
			sLod << "/L0" << lod;
		if (lod >= 10)
			sLod << "/L" << lod;
		//OE_WARN << LC << "sLod: "<<sLod.str() << std::endl;	
		return sLod.str();
	}



	std::string getDirectURI(const TileKey& key)
	{
		unsigned int tile_x, tile_y;
		key.getTileXY(tile_x, tile_y);
		unsigned int lod = key.getLevelOfDetail();
		char str[200];

		//天地图影像img_c
		/*lod += 1;
		sprintf(str,"http://t7.tianditu.com/DataServer?T=vec_c&x=%d&y=%d&l=%d\0", tile_x, tile_y,lod);
		*/
		//高德影像
		//sprintf(str,"http://webst01.is.autonavi.com/appmaptile?style=6&x=%d&y=%d&z=%d\0", tile_x, tile_y,lod);

		//arcgis影像
		//sprintf(str,"http://server.arcgisonline.com/ArcGIS/rest/services/ESRI_Imagery_World_2D/MapServer/tile/%d/%d/%d\0",lod, tile_y, tile_x);

		//谷歌影像
		//Google 地图使用了四个服务地址，即 http://mt ( 0—3).google.cn/......，都是可以用滴。
		//地图：http://mt2.google.cn/vt/lyrs=m@177000000&hl=zh-CN&gl=cn&src=app&。。。
		//影像底图：http://mt3.google.cn/vt/lyrs=s@110&hl=zh-CN&gl=cn&src=app&。。。
		//影像的叠加层：http://mt1.google.cn/vt/imgtp=png32&lyrs=h@177000000&hl=zh-CN&gl=cn&src=app&。。。
		//2) url地址中的 lyrs 表示的是图层类型，即瓦片类型，具体含义如下：

		//地图瓦片类型验证，直接修改lyrs参数的值即可：http://mt2.google.cn/vt/lyrs=y@258000000&hl=zh-CN&gl=CN&src=app&x=214130&y=114212&z=18&s=Ga
		//m：路线图  
		//t：地形图  
		//p：带标签的地形图  
		//s：卫星图  
		//y：带标签的卫星图  
		//h：标签层（路名、地名等）

#if 1
		std::string type = _options.key().get();//img_c
		sprintf(str, "http://mt2.google.cn/vt/lyrs=%s@258000000&hl=zh-CN&gl=CN&src=app&x=%d&y=%d&z=%d&s=Ga\0", type.c_str(),tile_x, tile_y, lod);
		return Stringify() << str;
#else
		std::string path = "F:/dataService/S57data/S57TileCache/myS57TileData/L%d/R%d/C%d.png";
		//path = Stringify() << path << lod << "/" << tile_x << "/" << tile_y << ".png";
		sprintf(str, path.c_str(), lod, tile_y, tile_x);
		return str;
#endif
	}
};


class BingTileSourceDriver : public TileSourceDriver
{
public:
	BingTileSourceDriver()
	{
		supportsExtension("osgearth_googlemap", "Microsoft Bing Driver");
	}

	virtual const char* className()
	{
		return "Microsoft Bing Driver";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return new BingTileSource(getTileSourceOptions(options));
	}
};

REGISTER_OSGPLUGIN(osgearth_googlemap, BingTileSourceDriver)
