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
#include <QtOpenGL/qgl.h>
#include "osgdb_osgearth_shp.h"
#include "mychartmaplib/imymap.h"
#include "mychartdatadriverlib/mychartsdatalib.h"
#include "mychartmaplib/imymapmanager.h"
#include "mychartmaplib/imylayer.h"
#include "mychartmaplib/imysymbol.h"
#include "mychartdrawlib/imys57map.h"
#include "mychartsymbollib/mychartsymbollib.h"
#include <QFileInfo>
#include <QUuid>
#include <QFile>

#pragma comment(lib,"Wininet.lib")

using namespace osgEarth;
using namespace osgEarth::Symbology;


std::string g_appRunDir = "";

#define LC "[Bing] "


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



class ShpTileSource : public TileSource
{
private:
	osgEarth::Drivers::ShpTileOptions _options;
	osg::ref_ptr<osgDB::Options>   _dbOptions;
	Random                         _prng;
	bool                           _debugDirect;
	osg::ref_ptr<Geometry>         _geom;
	osg::ref_ptr<osgText::Font>    _font;
	TileURICache                   _tileURICache;
	OpenThreads::Atomic            _apiCount;

	std::vector<std::string> _urls;
	bool mIsConnected;
	MyChart::IMyDrawable* mpShpData;

public:
	/**
	* Constructs the tile source
	*/
	ShpTileSource(const TileSourceOptions& options) :
		TileSource(options),
		_options(options),
		_debugDirect(true),
		_tileURICache(true, 1024u),
		mpShpData(NULL)
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
		MyChart::CMyChartCommonlib::S57DrawParameter.IsShowMapFrame = false;
	}

	/**
	* One-tile tile source initialization.
	*/
	Status initialize(const osgDB::Options* dbOptions)
	{
		// Always apply the NO CACHE policy.
		_dbOptions = Registry::instance()->cloneOrCreateOptions(dbOptions);

		// If the user did not specify an imagery set, default to aerial.
		if (!_options.imagerySet().isSet())
		{
			_options.imagerySet() = "Aerial";
		}

		const Profile* profile = Profile::create(
			SpatialReference::get("spherical-mercator"),
			MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
			1, 1);

		setProfile(profile);
		MyChart::CS57Symbollib::instance()->initial();
		MyChart::CMyShpDataDriver::initial();
		std::string  shpfile = _options.key().get();
		QString filePath = QString::fromStdString(shpfile);
		if (filePath.contains(".shp"))
		{
			MyChart::IMyLayer* pShpLayer = MyChart::CMyShpDataDriver::readShpLayer(shpfile.c_str());
			if (pShpLayer)
			{
				QFileInfo fileInfo = shpfile.c_str();
				MyChart::IMyGeneralMap* pShpMap = new MyChart::IMyGeneralMap(fileInfo.fileName());
				pShpMap->setFilePath(shpfile.c_str());
				pShpMap->getEnvelope() = pShpLayer->getEnvelope();
				pShpMap->getLayers().push_back(pShpLayer);
				this->mpShpData = pShpMap;

				if (pShpLayer->getLayerType() == "A")
				{
					MyChart::IMyPolygonSymbol* pSymbol = 
						dynamic_cast<MyChart::IMyPolygonSymbol*>(pShpLayer->getSymbol());
					if (pSymbol)
					{
						QColor fillColor(
							_options.color().r() * 255,
							_options.color().g() * 255,
							_options.color().b() * 255,
							_options.color().a() * 255);
						pSymbol->setFill(fillColor);

						QColor outlineColor(
							_options.borderColor().r() * 255,
							_options.borderColor().g() * 255,
							_options.borderColor().b() * 255,
							_options.borderColor().a() * 255);
						pSymbol->setColor(outlineColor);
						pSymbol->setWidth(_options.borderWidth().get());
					}
				}
				else if (pShpLayer->getLayerType() == "L")
				{
					MyChart::IMyLineSymbol* pSymbol = 
						dynamic_cast<MyChart::IMyLineSymbol*>(pShpLayer->getSymbol());
					if (pSymbol)
					{
						QColor outlineColor(
							_options.color().r() * 255,
							_options.color().g() * 255,
							_options.color().b() * 255,
							_options.color().a() * 255);
						pSymbol->setColor(outlineColor);
						pSymbol->setWidth(_options.borderWidth().get());
					}
				}
				else if (pShpLayer->getLayerType() == "P")
				{
					MyChart::IMyPointSymbol* pSymbol = 
						dynamic_cast<MyChart::IMyPointSymbol*>(pShpLayer->getSymbol());
					if (pSymbol)
					{
						QColor color(
							_options.color().r() * 255,
							_options.color().g() * 255,
							_options.color().b() * 255,
							_options.color().a() * 255);
						pSymbol->setColor(color);
						pSymbol->setSize(_options.borderWidth().get());
					}
				}
			}
		}
		else if (filePath.contains(".000"))
		{
			MyChart::IMyVectorMapData* pS57Data = MyChart::CMyChartS57DataDriver::readS57Map(shpfile.c_str());
			if (pS57Data)
			{
				QFileInfo fileInfo = shpfile.c_str();
				MyChart::IMyS57Map* pMyS57Map = new MyChart::IMyS57Map(fileInfo.fileName(),pS57Data);
				pMyS57Map->setFilePath(shpfile.c_str());
				pMyS57Map->getEnvelope() = pMyS57Map->getEnvelope();
				this->mpShpData = pMyS57Map;
			}
		}
		else
		{
			MyChart::CMyChartConfigDriver::fromFile(shpfile.c_str());
			this->mpShpData = MyChart::IMyMapManager::instance();
			
		}
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
#if 1
		double x, y;
		const osgEarth::GeoExtent& extend = key.getExtent();
        key.getExtent().getCentroid(x, y);
		OGREnvelope curBox;
		GeoPoint geo;

        GeoPoint( getProfile()->getSRS(), x, y ).transform(
            getProfile()->getSRS()->getGeographicSRS(),
            geo );

		GeoPoint(getProfile()->getSRS(), extend.xMin(), extend.yMax()).transform(
			getProfile()->getSRS()->getGeographicSRS(),
			geo);

		curBox.MinX = geo.x();
		curBox.MaxY = geo.y();

		GeoPoint(getProfile()->getSRS(), extend.xMax(), extend.yMin()).transform(
			getProfile()->getSRS()->getGeographicSRS(),
			geo);

		curBox.MaxX = geo.x();
		curBox.MinY = geo.y();
		int tileWidth = 256, tileHeight = 256;
		QColor backColor(0, 0,0, 0);
		MyChart::IMyDrawArgs drawArgs;
		drawArgs.Width = tileWidth;
		drawArgs.Height = tileHeight;
		QImage renderImage(drawArgs.Width, drawArgs.Height,QImage::Format_ARGB32);
		renderImage.fill(backColor);
		QPainter painter(&renderImage);
		painter.setRenderHint(QPainter::Antialiasing, true);

		GeoPoint geoLeftUp,geoRightDown;
		GeoPoint(getProfile()->getSRS(), extend.west(), extend.north()).transform(
			getProfile()->getSRS()->getGeographicSRS(),
			geoLeftUp);
		GeoPoint(getProfile()->getSRS(), extend.east(), extend.south()).transform(
			getProfile()->getSRS()->getGeographicSRS(),
			geoRightDown);

		drawArgs.geoLeftUpX = geoLeftUp.x();
		drawArgs.geoLeftUpY = geoLeftUp.y();
		drawArgs.geoRightDownX = geoRightDown.x();
		drawArgs.geoRightDownY = geoRightDown.y();
		drawArgs.CoorsystemType = MyChart::EMyCoorsystemType::MCT_Prj_MECATOR;

		MyChart::IMyCoorTransformer::geoCoor2PrjCoor(
			drawArgs.geoLeftUpX,
			drawArgs.geoLeftUpY,
			drawArgs.prjLeftUpX,
			drawArgs.prjLeftUpY,
			drawArgs.CoorsystemType);
		MyChart::IMyCoorTransformer::geoCoor2PrjCoor(
			drawArgs.geoRightDownX,
			drawArgs.geoRightDownY,
			drawArgs.prjRightDownX,
			drawArgs.prjRightDownY,
			drawArgs.CoorsystemType);
		double scale = 1;

		float extendDx = std::abs(drawArgs.prjRightDownX - drawArgs.prjLeftUpX);
		float extendDy = std::abs(drawArgs.prjLeftUpY - drawArgs.prjRightDownY);

		float extendRatio = extendDx / extendDy;
		float viewRatio = tileWidth / (float)tileHeight;
		if (viewRatio > extendRatio)
		{
			//以高度方向占满整个视口（每个像素代表的实际长度）
			scale = extendDy / (float)tileHeight;
		}
		else
		{
			//以宽度方向占满整个视口
			scale = extendDx / tileWidth;
		}
		drawArgs.Scale = scale;
		double pixelDis =0.3359375 / 1000.0;
		drawArgs.MapScale = pixelDis / scale;

		drawArgs.Painter = &painter;

		std::string cachePath = "";
		bool r = this->mpShpData->drawImmediately(&drawArgs);
		if (r)
		{
			QUuid id = QUuid::createUuid();
			QString strId = id.toString();

			std::string runPath = osgDB::getCurrentWorkingDirectory();
			cachePath = runPath + "/" + strId.toStdString() + ".png";
			renderImage.save(cachePath.c_str(),"png");
			osg::Image* pResult = URI(cachePath).getImage(_dbOptions.get(), progress);
			QFile::remove(cachePath.c_str());
			return pResult;
			/*QImage glImage = QGLWidget::convertToGLFormat(renderImage);

			unsigned char* data = new unsigned char[glImage.byteCount()];
			for (int i = 0; i < glImage.byteCount(); i++)
			{
				data[i] = glImage.bits()[i];
			}
			osg::Image *markerImage = new osg::Image();
			markerImage->setImage(glImage.width(), glImage.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);

			return markerImage;*/
			
		}
		cachePath = "systemImages/resources/image/noData.png";
		osg::Image* pResult = URI(cachePath).getImage(_dbOptions.get(), progress);
		return pResult;
#else
		QString url = this->getDirectURI(key).c_str();
		QImage qImage;
		if (osgDB::fileExists(url.toStdString())
			&& qImage.load(url))
		{
			QImage glImage = QGLWidget::convertToGLFormat(qImage);

			unsigned char* data = new unsigned char[glImage.byteCount()];
			for (int i = 0; i < glImage.byteCount(); i++)
			{
				data[i] = glImage.bits()[i];
			}
			osg::Image * pResult = new osg::Image();
			pResult->setImage(glImage.width(), glImage.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);
			return pResult;
		}
#endif
		return NULL;
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
		std::string path = this->_options.key().get();
		if (path == "")
			return "";
		path = path + "%d/%d/%d.png";
	
		sprintf(str, path.c_str(), lod, tile_x, tile_y);
		if (osgDB::fileExists(str))
		{
			return str;
		}
		else
		{
			unsigned maxDataLevel = _options.maxDataLevel().get();
			if (maxDataLevel >= lod)
				return osgDB::getCurrentWorkingDirectory() + "/systemImages/resources/image/noData.png";
			return "";
		}
	}
};


class ShpTileSourceDriver : public TileSourceDriver
{
public:
	ShpTileSourceDriver()
	{
		supportsExtension("osgearth_shp", "Microsoft Bing Driver");
	}

	virtual const char* className()
	{
		return "Microsoft Bing Driver";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return new ShpTileSource(getTileSourceOptions(options));
	}
};

REGISTER_OSGPLUGIN(osgearth_shp, ShpTileSourceDriver)
