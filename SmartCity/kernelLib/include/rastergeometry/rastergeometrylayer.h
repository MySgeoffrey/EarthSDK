#ifndef VR_RASTERGEOMETRYLAYER_H
#define VR_RASTERGEOMETRYLAYER_H

#include <osgEarth/ImageLayer>
#include "rastergeometry/rastergeometryconfig.h"

namespace Raster
{
	class VR_RASTERGEOMETRY_EXPORT CRasterGeometryLayer
		: public osgEarth::ImageLayer
	{
	public:
		CRasterGeometryLayer( const osgEarth::ImageLayerOptions& options );

		CRasterGeometryLayer( const std::string& name, const osgEarth::TileSourceOptions& driverOptions );

		CRasterGeometryLayer( const osgEarth::ImageLayerOptions& options, osgEarth::TileSource* tileSource );

		virtual ~CRasterGeometryLayer();

		virtual void refresh();
	};
}

#endif //VR_RASTERGEOMETRYLAYER_H