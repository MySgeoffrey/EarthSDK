// osgdb_osgearth_bingd.cpp : 定义 DLL 应用程序的导出函数。
//

#include <osgEarthDrivers/bing/BingOptions>

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

using namespace osgEarth;
using namespace osgEarth::Symbology;

namespace osgEarth { namespace Drivers
{
    using namespace osgEarth;

    /**
     * Configuration structure for the Microsoft Bing driver.
     * http://www.bing.com/developers/
     *
     * Using Bing requires an API key. You can get one from the URL above.
     * You are responsible for complying with the Bing terms of service.
     */
    class ShpTileOptions : public TileSourceOptions // NO EXPORT; header only
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

		Color& borderColor() { return _borderColor; }
		const Color& borderColor() const { return _borderColor; }

		Color& color() { return _color; }
		const Color& color() const { return _color; }

		/** Line border rendering width. */
		optional<double>& borderWidth() { return _borderWidth; }
		const optional<double>& borderWidth() const { return _borderWidth; }
    public:
        /**
         * Constructs a Bing configuration structure.
         */
        ShpTileOptions( const TileSourceOptions& opt =TileSourceOptions() ) : TileSourceOptions( opt ),
            _imagerySet        ( "Aerial" ),
            _imageryMetadataAPI( "http://dev.virtualearth.net/REST/v1/Imagery/Metadata" ),
			 _borderColor(Color::White),
            _borderWidth(1)
        {
            setDriver( "shp" );
            fromConfig( _conf );
        }

        /** dtor */
        virtual ~ShpTileOptions() { }

    public:
        Config getConfig() const {
            Config conf = TileSourceOptions::getConfig();
            conf.updateIfSet("key",                  _apiKey);
            conf.updateIfSet("imagery_set",          _imagerySet );
			conf.updateIfSet("imagery_metadata_api", _imageryMetadataAPI );
			conf.add("borderColor", _borderColor.toHTML());
			conf.add("color", _color.toHTML());
			conf.updateIfSet("borderWidth", _borderWidth);
            return conf;
        }

    protected:
        void mergeConfig( const Config& conf ) {
            TileSourceOptions::mergeConfig( conf );
            fromConfig( conf );
        }

    private:
        void fromConfig( const Config& conf ) {
            conf.getIfSet("key",                  _apiKey);
            conf.getIfSet("imagery_set",          _imagerySet );
            conf.getIfSet("imagery_metadata_api", _imageryMetadataAPI );
			_borderColor = Color(conf.value("borderColor"));
			_color =  Color(conf.value("color"));
			conf.getIfSet("borderWidth", _borderWidth);
        }

        optional<std::string> _apiKey;
        optional<std::string> _imagerySet;
        optional<std::string> _imageryMetadataAPI;
		 //w.g.描边的颜色和宽度(单位像素)
        Color            _borderColor;
		Color			 _color;
        optional<double>  _borderWidth;
    };

} } // namespace osgEarth::Drivers

