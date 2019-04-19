#ifndef VR_RASTERGEOMETRYTILESOURCE_H
#define VR_RASTERGEOMETRYTILESOURCE_H

#include <osgEarth/ImageLayer>
#include "rastergeometry/rastergeometryconfig.h"

namespace Raster
{
	class VR_RASTERGEOMETRY_EXPORT CRasterGeometryTileSource
		: public osgEarth::TileSource
	{
	public:
		CRasterGeometryTileSource(const osgEarth::TileSourceOptions& options);

		virtual ~CRasterGeometryTileSource();

		osgEarth::Status initialize(const osgDB::Options* dbOptions);

		osgEarth::CachePolicy getCachePolicyHint(const osgEarth::Profile*) const;

		osg::Image* createImage(const osgEarth::TileKey& key, osgEarth::ProgressCallback* progress);

		virtual bool isDynamic() const ;
	};
}

#endif //VR_RASTERGEOMETRYTILESOURCE_H