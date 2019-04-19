#include "rastergeometry/rastergeometrylayer.h"


namespace Raster
{
	CRasterGeometryLayer::CRasterGeometryLayer( const osgEarth::ImageLayerOptions& options )
		: osgEarth::ImageLayer(options)
	{
	
	}

	CRasterGeometryLayer::CRasterGeometryLayer( const std::string& name, const osgEarth::TileSourceOptions& driverOptions )
		: osgEarth::ImageLayer(name,driverOptions)
	{
	
	}

	CRasterGeometryLayer::CRasterGeometryLayer( const osgEarth::ImageLayerOptions& options, osgEarth::TileSource* tileSource )
		: osgEarth::ImageLayer(options,tileSource)
	{
	
	}

	CRasterGeometryLayer::~CRasterGeometryLayer()
	{
	
	}

	void CRasterGeometryLayer::refresh()
	{
		fireCallback( &osgEarth::TerrainLayerCallback::onVisibleChanged );
	}
}
