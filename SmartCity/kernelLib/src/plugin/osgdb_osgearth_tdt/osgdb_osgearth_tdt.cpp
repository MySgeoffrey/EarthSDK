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
		class TDTOptions : public TileSourceOptions // NO EXPORT; header only
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
			TDTOptions(const TileSourceOptions& opt = TileSourceOptions()) : TileSourceOptions(opt),
				_imagerySet("Aerial"),
				_imageryMetadataAPI("http://dev.virtualearth.net/REST/v1/Imagery/Metadata")
			{
				setDriver("tdt");
				fromConfig(_conf);
			}

			/** dtor */
			virtual ~TDTOptions() { }

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
	osgEarth::Drivers::TDTOptions _options;
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
		const Profile* profile = Profile::create(
			spatialReference,
			-180, -90, 180, 90,
			2, 1);

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
		unsigned int tile_x,tile_y;
		key.getTileXY(tile_x,tile_y);
		unsigned int lod = key.getLevelOfDetail();
		lod += 0;
		std::stringstream ss;
		
		ss<< lod ;
		
		char str[200];
		//天地图影像img_c
		lod += 1;
		//vec_c
		std::string type = _options.key().get();//img_c
		sprintf(str,"http://t7.tianditu.com/DataServer?T=%s&x=%d&y=%d&l=%d\0",type.c_str(),tile_x, tile_y,lod);

		return Stringify() << str;
	}
};


class BingTileSourceDriver : public TileSourceDriver
{
public:
	BingTileSourceDriver()
	{
		supportsExtension("osgearth_tdt", "Microsoft Bing Driver");
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
