#include "globemap/globemap.h"
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include "tilesource//osgdb_osgearth_googlemap.h"
#include "tilesource/osgdb_osgearth_customtile.h"
#include "tilesource/osgdb_osgearth_local_google84.h"
#include "tilesource/osgdb_osgearth_local_googleMecator.h"
#include "tilesource/osgdb_osgearth_tdt.h"
#include "tilesource/osgdb_osgearth_shp.h"
#include "tilesource/osgdb_osgearth_arcgistile.h"
#include "tilesource/osgdb_osgearth_gaodemap.h"

using namespace Globe;
using namespace osgEarth::Drivers;

//void addShpByAGG()
//{
//	// Next we add a feature layer. 
//	OGRFeatureOptions featureOptions;
//	// Configures the feature driver to load the vectors from a shapefile:
//	featureOptions.url() = "world.shp";
//
//	// Define a style for the feature data. Since we are going to render the
//	// vectors as lines, configure the line symbolizer:
//	Style style;
//	LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
//	ls->stroke()->color() = Color::Yellow;
//	ls->stroke()->width() = 6.0f;
//	ls->stroke()->lineCap() = osgEarth::Symbology::Stroke::LINECAP_ROUND;//使连接处圆滑一些
//
//	AGGLiteOptions rasterOptions;
//	rasterOptions.featureOptions() = featureOptions;
//	rasterOptions.styles() = new StyleSheet();
//	rasterOptions.styles()->addStyle(style);
//	rasterOptions.borderColor() = Color::Red;//包边颜色
//	rasterOptions.borderWidth() = 3.0f;//包边宽度（单位像素）
//	map->addImageLayer(new ImageLayer("my features", rasterOptions));
//}

CGlobeMap::CGlobeMap()
{
	mpEarthMap = NULL;
	mpLayerContainer = new Core::IObjectContainer;
}

CGlobeMap::~CGlobeMap()
{
	mpEarthMap = NULL;
	if(NULL != mpLayerContainer)
	{    
		mpLayerContainer->removeAndDestroyAllObjects();
		mpLayerContainer = NULL;
	}
}

bool CGlobeMap::getImageLayers( osgEarth::ImageLayerVector& out_layers ) const
{
	if(NULL == mpEarthMap)
		return false;
	mpEarthMap->getImageLayers(out_layers);
	return true;
}

int CGlobeMap::getNumImageLayers() const
{
	if(NULL == mpEarthMap)
		return -1;
	return mpEarthMap->getNumImageLayers();
}

osgEarth::ImageLayer* CGlobeMap::getImageLayerByName( const std::string& name ) const
{
	if(NULL == mpEarthMap)
		return NULL;
	return mpEarthMap->getImageLayerByName(name);
}

bool CGlobeMap::getElevationLayers( osgEarth::ElevationLayerVector& out_layers ) const
{
	if(NULL == mpEarthMap)
		return false;
	mpEarthMap->getElevationLayers(out_layers);
	return true;
}

int CGlobeMap::getNumElevationLayers() const
{
	if(NULL == mpEarthMap)
		return -1;
	return mpEarthMap->getNumElevationLayers();
}

osgEarth::ElevationLayer* CGlobeMap::getElevationLayerByName( const std::string& name ) const
{
	if(NULL == mpEarthMap)
		return NULL;
	return mpEarthMap->getElevationLayerByName(name);
}

bool CGlobeMap::getModelLayers( osgEarth::ModelLayerVector& out_layers ) const
{
	if(NULL == mpEarthMap)
		return false;
	mpEarthMap->getModelLayers(out_layers);
	return true;
}

int CGlobeMap::getNumModelLayers() const
{
	if(NULL == mpEarthMap)
		return -1;
	return mpEarthMap->getNumModelLayers();
}

osgEarth::ModelLayer* CGlobeMap::getModelLayerByName( const std::string& name ) const
{
	if(NULL == mpEarthMap)
		return NULL;
	return mpEarthMap->getModelLayerByName(name);
}

osgEarth::ImageLayer* CGlobeMap::addImageLayer( 
	const std::string& imageUrl,
	const std::string& layerName,
	const MapType& mapType,
	const int& minLevel,
	const int& maxLevel)
{
	if(NULL == mpEarthMap)
		return NULL;
	
	ImageLayer* pImageLayer = mpEarthMap->getImageLayerByName(layerName);
	if (pImageLayer)
		return pImageLayer;
	//备注：构造osgEarth::ImageLayer时，如果传入图层名称参数，则最小级别不起作用
	//因为，图层名称通过option传入，而构造osgEarth::ImageLayer时不传图层名称
	if(mapType == MapType::TMS_MT)
	{
		TMSOptions imagery;
		imagery.url() = imageUrl;
		//imagery.tmsType() = "google";
		imagery.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,imagery);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new ImageLayer(imageLayerOption);
	}
	else if(mapType == MapType::WEB_GOOGLE_MT)
	{
		osgEarth::Drivers::GooglemapOptions googleImageOption;
		googleImageOption.key() = "s";
		googleImageOption.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,googleImageOption);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(layerName,imageLayerOption);
	}
	else if(mapType == MapType::WEB_TDT_IMAGE_MT)
	{
		osgEarth::Drivers::TDTOptions tdtOptions;
		tdtOptions.key() = "img_c";
		tdtOptions.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,tdtOptions);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(layerName,imageLayerOption);
	}
	else if(mapType == MapType::WEB_TDT_MAP_MT)
	{
		osgEarth::Drivers::TDTOptions tdtOptions;
		tdtOptions.key() = "vec_c";
		tdtOptions.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,tdtOptions);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(layerName,imageLayerOption);
	}
	else if(mapType == MapType::CUSTOM_MT)
	{
		osgEarth::Drivers::CustomtileOptions customImageOption;
		customImageOption.key() = imageUrl;
		customImageOption.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,customImageOption);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(imageLayerOption);
	}
	else if(mapType == MapType::ARCGIS_TILE)
	{
		osgEarth::Drivers::ArcgisTileOptions arcgismageOption;
		arcgismageOption.key() = imageUrl;
		arcgismageOption.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,arcgismageOption);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(imageLayerOption);
	}
	else if(mapType == MapType::GOOGLE_LOCAL84)
	{
		osgEarth::Drivers::Local_google84Options local_google84Options;
		local_google84Options.key() = imageUrl;
		local_google84Options.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,local_google84Options);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(imageLayerOption);
	}
	else if(mapType == MapType::GOOGLE_LOCALMECATOR)
	{
		osgEarth::Drivers::Local_googleMecatorOptions local_googleMecatorOptions;
		local_googleMecatorOptions.key() = imageUrl;
		local_googleMecatorOptions.maxDataLevel() = maxLevel;
		osgEarth::ImageLayerOptions imageLayerOption(layerName,local_googleMecatorOptions);
		imageLayerOption.minLevel() = minLevel;
		imageLayerOption.maxLevel() = maxLevel;
		pImageLayer = new osgEarth::ImageLayer(imageLayerOption);
	}
	else if (mapType == WEB_GAODE_MAP_MT)
	{
		//////gaoDe影像
		osgEarth::Drivers::GaoDemapOptions gaoDeImageOption;
		gaoDeImageOption.maxDataLevel() = 18;
		gaoDeImageOption.key() = "7";
		pImageLayer = new osgEarth::ImageLayer("GaoDeMapLayer", gaoDeImageOption);
	}
	mpEarthMap->addImageLayer(pImageLayer);
	return pImageLayer;
}

void CGlobeMap::removeImageLayer( osgEarth::ImageLayer* layer )
{
	if(NULL == mpEarthMap || NULL == layer)
		return;
	mpEarthMap->removeImageLayer(layer);
}

osgEarth::ElevationLayer* CGlobeMap::addElevationLayer( const std::string& elevationUrl,const std::string& layerName )
{
	if(NULL == mpEarthMap)
		return NULL;

	ElevationLayer* pElevationLayer = NULL;
	//if(mapType == MapType::TMS_MT)
	{
		TMSOptions elevation;
		elevation.url() = elevationUrl;
		pElevationLayer = new ElevationLayer(layerName, elevation);
		mpEarthMap->addElevationLayer(pElevationLayer);
	}
	//else if(mapType == MapType::GOOGLE_MT)
	//{
	//	GooglemapOptions elevation;
	//	elevation.key() = elevationUrl;
	//	pElevationLayer = new ElevationLayer(layerName, elevation);
	//	mpEarthMap->addElevationLayer(pElevationLayer);
	//}

	return pElevationLayer;
}

void CGlobeMap::removeElevationLayer( osgEarth::ElevationLayer* layer )
{
	if(NULL == mpEarthMap || NULL == layer)
		return;
	mpEarthMap->removeElevationLayer(layer);
}

osgEarth::ModelLayer* CGlobeMap::addPointShpModelLayer( 
	const std::string& shpPath, const std::string& layerName,
	const osgEarth::Symbology::Color& textColor, const float& textSize,const std::string& font,const std::string& contextField )
{
	if(NULL == mpEarthMap)
		return NULL;
	// Next we add a feature layer. 
	OGRFeatureOptions featureOptions;
	// Configures the feature driver to load the vectors from a shapefile:
	featureOptions.url() = shpPath;

	// Define a style for the feature data. Since we are going to render the
	// vectors as lines, configure the line symbolizer:
	Style style;
	std::string useFont = "";
	if(font == "")
		useFont = "fonts/simhei.ttf";
	else
		useFont = font;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= useFont;//指定中文字体路径
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = textSize;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = textColor;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->content() = contextField;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
	style.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;

	FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new StyleSheet();
	geomOptions.styles()->addStyle( style );
	geomOptions.enableLighting() = false;
	FeatureDisplayLayout layout;
	layout.tileSizeFactor() = 52.0;
	layout.addLevel( FeatureLevel(0, 1000000000.0f, style.getName()) );
	geomOptions.layout() = layout;

	ModelLayerOptions layerOptions( layerName, geomOptions );
	ModelLayer* pModelLayer = new ModelLayer(layerOptions);

	mpEarthMap->addModelLayer(pModelLayer);
	return pModelLayer;
}

osgEarth::ModelLayer* CGlobeMap::addLineShpModelLayer( 
	const std::string& shpPath,const std::string& layerName, const osgEarth::Symbology::Color& lineColor,const float& lineWidth )
{
	if(NULL == mpEarthMap)
		return NULL;
	// Next we add a feature layer. 
	OGRFeatureOptions featureOptions;
	// Configures the feature driver to load the vectors from a shapefile:
	featureOptions.url() = shpPath;

	// Define a style for the feature data. Since we are going to render the
	// vectors as lines, configure the line symbolizer:
	Style style;

	LineSymbol* ls = style.getOrCreateSymbol<LineSymbol>();
	ls->stroke()->color() = lineColor;
	ls->stroke()->width() = lineWidth;
	style.getOrCreate<RenderSymbol>()->depthOffset()->enabled() = true;
	style.getOrCreate<RenderSymbol>()->depthOffset()->minBias() = 3;

	FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new StyleSheet();
	geomOptions.styles()->addStyle( style );
	geomOptions.enableLighting() = false;
	FeatureDisplayLayout layout;
	layout.tileSizeFactor() = 52.0;
	layout.addLevel( FeatureLevel(0, 1000000000.0f, style.getName()) );
	geomOptions.layout() = layout;

	ModelLayerOptions layerOptions( layerName, geomOptions );
	ModelLayer* pModelLayer = new ModelLayer(layerOptions);

	mpEarthMap->addModelLayer(pModelLayer);
	return pModelLayer;
}

osgEarth::ModelLayer* CGlobeMap::addPolygonShpModelLayer( 
	const std::string& shpPath,const std::string& layerName, const osgEarth::Symbology::Color& polygonColor,
	const osgEarth::Symbology::Color& outLineColor,const float& outLineWidth )
{
	if(NULL == mpEarthMap)
		return NULL;
	// Next we add a feature layer. 
	OGRFeatureOptions featureOptions;
	// Configures the feature driver to load the vectors from a shapefile:
	featureOptions.url() = shpPath;

	// Define a style for the feature data. Since we are going to render the
	// vectors as lines, configure the line symbolizer:
	Style style;
	style.getOrCreate<LineSymbol>()->stroke()->color() = outLineColor;
	style.getOrCreate<LineSymbol>()->stroke()->width() = outLineWidth;
	style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
		osgEarth::Symbology::Color(polygonColor);
	//style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 50.0;
	style.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
	style.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
	style.getOrCreate<RenderSymbol>()->depthOffset()->enabled() = true;
	style.getOrCreate<RenderSymbol>()->depthOffset()->minBias() = 1000;

	FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new StyleSheet();
	geomOptions.styles()->addStyle( style );
	geomOptions.enableLighting() = false;

	FeatureDisplayLayout layout;
	layout.tileSizeFactor() = 52.0;
	layout.addLevel( FeatureLevel(0, 1000000000.0f, style.getName()) );
	geomOptions.layout() = layout;

	ModelLayerOptions layerOptions( layerName, geomOptions );
	ModelLayer* pModelLayer = new ModelLayer(layerOptions);

	mpEarthMap->addModelLayer(pModelLayer);
	return pModelLayer;
}

osgEarth::ImageLayer* CGlobeMap::addCFGShpModelLayer(const std::string& shpPath,const std::string& layerName,const int& minLevel, const int& maxLevel)
{
	ShpTileOptions featureOptions;
	featureOptions.key() = shpPath;
	osgEarth::ImageLayerOptions imageLayerOption(layerName,featureOptions);
	imageLayerOption.minLevel() = minLevel;
	imageLayerOption.maxLevel() = maxLevel;
	osgEarth::ImageLayer* pLayer = new ImageLayer(layerName, imageLayerOption);
	this->mpEarthMap->addImageLayer(pLayer);
	return pLayer;
}


void CGlobeMap::removeModelLayer( osgEarth::ModelLayer* layer )
{
	if(NULL == mpEarthMap || NULL == layer)
		return;
	mpEarthMap->addModelLayer(layer);
}

CGlobeMap* CGlobeMap::getInstance()
{
	if(NULL == mpInstance)
		mpInstance = new CGlobeMap;
	return mpInstance;
}

void CGlobeMap::releseInstance()
{
	if(NULL != mpInstance)
		delete mpInstance;
	mpInstance = NULL;
}

void CGlobeMap::initialise( osgEarth::Map* earthMap )
{
	mpEarthMap = earthMap;
}

CGlobeMap* CGlobeMap::mpInstance = NULL;

DynamicObject::CDynamicObjectLayer* Globe::CGlobeMap::addDynamicObjectLayer( 
	const std::string& id,const std::string& layerName,osgEarth::MapNode* pMapNode  )
{
	DynamicObject::CDynamicObjectLayer* pDynamicObjectLayer = new DynamicObject::CDynamicObjectLayer(pMapNode);
	if(NULL == pDynamicObjectLayer)
		return pDynamicObjectLayer;
	pDynamicObjectLayer->setID(id);
	pDynamicObjectLayer->setName(layerName);
	mpLayerContainer->addObject(pDynamicObjectLayer);
	return pDynamicObjectLayer;
}

DynamicObject::CDynamicObjectLayer* Globe::CGlobeMap::getDynamicObjectLayerByID( const std::string& id )
{
	return dynamic_cast<DynamicObject::CDynamicObjectLayer*>(mpLayerContainer->getObject(id));
}

DynamicObject::CDynamicObjectLayer* Globe::CGlobeMap::getDynamicObjectLayerByName( const std::string& name )
{
	std::vector<std::string> keys;
	mpLayerContainer->getKeys(keys);
	Common::IObject* pObject = NULL;
	for (int index = 0; index < keys.size(); ++index)
	{
		pObject = mpLayerContainer->getObject(keys.at(index));
		if(NULL == pObject)
			continue;
		if(name == pObject->getName())
			return dynamic_cast<DynamicObject::CDynamicObjectLayer*>(pObject);
	}
	return NULL;
}

bool Globe::CGlobeMap::removeDynamicObjectLayerByID( const std::string& id )
{
	Common::IObject* pObject = mpLayerContainer->removeObject(id);
	if(NULL == pObject)
		return false;
	delete pObject;
	pObject = NULL;
	return true;
}

bool Globe::CGlobeMap::removeDynamicObjectLayerByName( const std::string& name )
{
	DynamicObject::CDynamicObjectLayer* pDynamicObjectLayer = getDynamicObjectLayerByName(name);
	if(NULL == pDynamicObjectLayer)
		return false;
	return removeDynamicObjectLayerByID(pDynamicObjectLayer->getID());
}

DynamicObject::CSceneObjectLayer* Globe::CGlobeMap::addSceneObjectLayer( 
	const std::string& id,const std::string& layerName,const std::string& layerType )
{
	DynamicObject::CSceneObjectLayer* pSceneObjectLayer = new DynamicObject::CSceneObjectLayer(layerType);
	if(NULL == pSceneObjectLayer)
		return pSceneObjectLayer;
	pSceneObjectLayer->setID(id);
	pSceneObjectLayer->setName(layerName);
	mpLayerContainer->addObject(pSceneObjectLayer);
	return pSceneObjectLayer;
}

DynamicObject::CSceneObjectLayer* Globe::CGlobeMap::getSceneObjectLayerByID( const std::string& id )
{
	return dynamic_cast<DynamicObject::CSceneObjectLayer*>(mpLayerContainer->getObject(id));
}

DynamicObject::CSceneObjectLayer* Globe::CGlobeMap::getSceneObjectLayerByName( const std::string& name )
{
	std::vector<std::string> keys;
	mpLayerContainer->getKeys(keys);
	Common::IObject* pObject = NULL;
	for (int index = 0; index < keys.size(); ++index)
	{
		pObject = mpLayerContainer->getObject(keys.at(index));
		if(NULL == pObject)
			continue;
		if(name == pObject->getName())
			return dynamic_cast<DynamicObject::CSceneObjectLayer*>(pObject);
	}
	return NULL;
}

bool Globe::CGlobeMap::removeSceneObjectLayerByID( const std::string& id )
{
	Common::IObject* pObject = mpLayerContainer->removeObject(id);
	if(NULL == pObject)
		return false;
	delete pObject;
	pObject = NULL;
	return true;
}

bool Globe::CGlobeMap::removeSceneObjectLayerByName( const std::string& name )
{
	DynamicObject::CSceneObjectLayer* pSceneObjectLayer = getSceneObjectLayerByName(name);
	if(NULL == pSceneObjectLayer)
		return false;
	return removeSceneObjectLayerByID(pSceneObjectLayer->getID());
}

osgEarth::Map* Globe::CGlobeMap::getMapPtr()
{
	return this->mpEarthMap;
}

const std::string Globe::CSceneObjectLayerType::WARNREGION = "CSceneObjectLayerType::WARNREGION";

const std::string Globe::CSceneObjectLayerType::NOFLYREGION = "CSceneObjectLayerType::NOFLYREGION";

const std::string Globe::CSceneObjectLayerType::SCANREGION = "CSceneObjectLayerType::SCANREGION";
