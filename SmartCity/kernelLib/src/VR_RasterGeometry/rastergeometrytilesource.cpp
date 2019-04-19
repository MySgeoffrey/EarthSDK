#include "rastergeometry/rastergeometrytilesource.h"

using namespace osgEarth;

namespace Raster
{
	CRasterGeometryTileSource::CRasterGeometryTileSource(
		const osgEarth::TileSourceOptions& options)
		: osgEarth::TileSource(options)
	{
	
	}

	CRasterGeometryTileSource::~CRasterGeometryTileSource()
	{
	
	}

	osgEarth::Status CRasterGeometryTileSource::initialize(const osgDB::Options* dbOptions)
	{
		const osgEarth::Profile* profile = osgEarth::Profile::create(
			osgEarth::SpatialReference::get("spherical-mercator"),
			MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
			1, 1);
		setProfile(profile);

		return osgEarth::STATUS_OK;
	}

	osgEarth::CachePolicy CRasterGeometryTileSource::getCachePolicyHint(const osgEarth::Profile*) const
	{
		return osgEarth::CachePolicy::NO_CACHE;//
	}

	osg::Image* CRasterGeometryTileSource::createImage(const osgEarth::TileKey& key, osgEarth::ProgressCallback* progress)
	{
		std::string url = "F:/test.png";
		return osgDB::readImageFile(url);
	}

	bool CRasterGeometryTileSource::isDynamic() const
	{
		return true;
	}
}
