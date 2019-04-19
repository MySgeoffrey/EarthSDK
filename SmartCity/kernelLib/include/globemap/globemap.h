#if !defined(_GLOBEMAP_H)
#define _GLOBEMAP_H

#include "globemap/globemapconfig.h"
#include "framework/common/iobject.h"
#include "osgEarth/ImageLayer"
#include "osgEarth/ElevationLayer"
#include "osgEarth/ModelLayer"
#include "osgEarth/MaskLayer"
#include "osgEarth/MapCallback"
#include "osgEarthSymbology/Color"
#include "osgEarth/Map"
#include "dynamicobject/dynamicobjectlayer.h"
#include "dynamicobject/sceneobjectlayer.h"

namespace Globe
{
	class VR_GLOBEMAP_EXPORT CSceneObjectLayerType
	{
	public:
		static const std::string WARNREGION;
		static const std::string NOFLYREGION;
		static const std::string SCANREGION;
	};

	//地图格式
	enum MapType
	{
		//谷歌格式
		WEB_GOOGLE_MT,

		//天地图影像
		WEB_TDT_IMAGE_MT,

		//天地图矢量
		WEB_TDT_MAP_MT,

		//高德影像
		WEB_GAODE_IMAGE_MT,

		//高德矢量
		WEB_GAODE_MAP_MT,

		//标准TMS格式
		TMS_MT,

		//自定义格式
		CUSTOM_MT,

		//arcgis松散瓦片
		ARCGIS_TILE,

		//本地谷歌84影像
		GOOGLE_LOCAL84,

		//本地谷歌墨卡托影像
		GOOGLE_LOCALMECATOR
	};

	class VR_GLOBEMAP_EXPORT CGlobeMap : public Common::IObject
	{
	public:
		/// <summary>
		/// 图层管理单例对象
		/// </summary>
		static CGlobeMap* getInstance();

		void releseInstance();

		/// <summary>
		/// 初始化图层管理
		/// </summary>
		/// <param name="earthMap">osgEarth地图对象</param>
		void initialise(osgEarth::Map* earthMap);

		/// <summary>
		/// 获取瓦片图层集合
		/// </summary>
		/// <param name="out_layers">瓦片地址</param>
		/// <returns>是否获取成功</returns>
		bool getImageLayers( osgEarth::ImageLayerVector& out_layers ) const;

		/// <summary>
		/// 获取瓦片图层个数
		/// </summary>
		/// <returns>图层个数</returns>
		int getNumImageLayers() const;

		/// <summary>
		/// 根据名称获取瓦片图层对象
		/// </summary>
		/// <param name="name">图层名称</param>
		/// <returns>图层对象</returns>
		osgEarth::ImageLayer* getImageLayerByName( const std::string& name ) const;

		bool getElevationLayers( osgEarth::ElevationLayerVector& out_layers ) const;

		int getNumElevationLayers() const;

		osgEarth::ElevationLayer* getElevationLayerByName( const std::string& name ) const;

		bool getModelLayers( osgEarth::ModelLayerVector& out_layers ) const;

		int getNumModelLayers() const;

		osgEarth::ModelLayer* getModelLayerByName( const std::string& name ) const;

		/// <summary>
		/// 添加瓦片图层接口
		/// </summary>
		/// <param name="imageUrl">瓦片地址</param>
		/// <param name="layerName">图层名称</param>
		/// <param name="mapType">瓦片地图类型</param>
		/// <param name="minLevel">最小显示级别</param>
		/// <param name="maxLevel">最大显示级别</param>
		/// <returns>图层对象</returns>
		osgEarth::ImageLayer* addImageLayer(
			const std::string& imageUrl,
			const std::string& layerName,
			const MapType& mapType,
			const int& minLevel = 0,
			const int& maxLevel = 18);

		void removeImageLayer( osgEarth::ImageLayer* layer );

		osgEarth::ElevationLayer* addElevationLayer(const std::string& elevationUrl,const std::string& layerName);

		void removeElevationLayer( osgEarth::ElevationLayer* layer );

		//图标，中文的支持
		osgEarth::ModelLayer* addPointShpModelLayer(const std::string& shpPath,
			const std::string& layerName,const osgEarth::Symbology::Color& textColor,
			const float& textSize,const std::string& font,const std::string& contextField);

		osgEarth::ModelLayer* addLineShpModelLayer(const std::string& shpPath,const std::string& layerName,
			const osgEarth::Symbology::Color& lineColor,const float& lineWidth);

		//关于高程模式
		osgEarth::ModelLayer* addPolygonShpModelLayer(const std::string& shpPath,const std::string& layerName,
			const osgEarth::Symbology::Color& polygonColor,
			const osgEarth::Symbology::Color& outLineColor,const float& outLineWidth);

		osgEarth::ImageLayer* addCFGShpModelLayer(const std::string& shpPath,const std::string& layerName,const int& minLevel  = 0, const int& maxLevel = 15);

		void removeModelLayer( osgEarth::ModelLayer* layer );

		DynamicObject::CDynamicObjectLayer* addDynamicObjectLayer(
			const std::string& id,const std::string& layerName,osgEarth::MapNode* pMapNode);

		DynamicObject::CDynamicObjectLayer* getDynamicObjectLayerByID(const std::string& id);

		DynamicObject::CDynamicObjectLayer* getDynamicObjectLayerByName(const std::string& name);

		bool removeDynamicObjectLayerByID(const std::string& id);

		bool removeDynamicObjectLayerByName(const std::string& name);

		DynamicObject::CSceneObjectLayer* addSceneObjectLayer(
			const std::string& id,const std::string& layerName,const std::string& layerType);

		DynamicObject::CSceneObjectLayer* getSceneObjectLayerByID(const std::string& id);

		DynamicObject::CSceneObjectLayer* getSceneObjectLayerByName(const std::string& name);

		bool removeSceneObjectLayerByID(const std::string& id);

		bool removeSceneObjectLayerByName(const std::string& name);

		osgEarth::Map* getMapPtr();
	private:
		CGlobeMap();

		virtual ~CGlobeMap();

		static CGlobeMap* mpInstance;

		osgEarth::Map* mpEarthMap;

		Core::IObjectContainer* mpLayerContainer;
	};
}

#endif