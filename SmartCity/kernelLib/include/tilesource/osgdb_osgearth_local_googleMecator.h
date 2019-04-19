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
    class Local_googleMecatorOptions : public TileSourceOptions // NO EXPORT; header only
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
        Local_googleMecatorOptions( const TileSourceOptions& opt =TileSourceOptions() ) : TileSourceOptions( opt ),
            _imagerySet        ( "Aerial" ),
            _imageryMetadataAPI( "http://dev.virtualearth.net/REST/v1/Imagery/Metadata" )
        {
            setDriver( "local_googleMecator" );
            fromConfig( _conf );
        }

        /** dtor */
        virtual ~Local_googleMecatorOptions() { }

    public:
        Config getConfig() const {
            Config conf = TileSourceOptions::getConfig();
            conf.updateIfSet("key",                  _apiKey);
            conf.updateIfSet("imagery_set",          _imagerySet );
            conf.updateIfSet("imagery_metadata_api", _imageryMetadataAPI );
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
        }

        optional<std::string> _apiKey;
        optional<std::string> _imagerySet;
        optional<std::string> _imageryMetadataAPI;
    };

} } // namespace osgEarth::Drivers

