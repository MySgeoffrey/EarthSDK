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

	//��ͼ��ʽ
	enum MapType
	{
		//�ȸ��ʽ
		WEB_GOOGLE_MT,

		//���ͼӰ��
		WEB_TDT_IMAGE_MT,

		//���ͼʸ��
		WEB_TDT_MAP_MT,

		//�ߵ�Ӱ��
		WEB_GAODE_IMAGE_MT,

		//�ߵ�ʸ��
		WEB_GAODE_MAP_MT,

		//��׼TMS��ʽ
		TMS_MT,

		//�Զ����ʽ
		CUSTOM_MT,

		//arcgis��ɢ��Ƭ
		ARCGIS_TILE,

		//���عȸ�84Ӱ��
		GOOGLE_LOCAL84,

		//���عȸ�ī����Ӱ��
		GOOGLE_LOCALMECATOR
	};

	class VR_GLOBEMAP_EXPORT CGlobeMap : public Common::IObject
	{
	public:
		/// <summary>
		/// ͼ�����������
		/// </summary>
		static CGlobeMap* getInstance();

		void releseInstance();

		/// <summary>
		/// ��ʼ��ͼ�����
		/// </summary>
		/// <param name="earthMap">osgEarth��ͼ����</param>
		void initialise(osgEarth::Map* earthMap);

		/// <summary>
		/// ��ȡ��Ƭͼ�㼯��
		/// </summary>
		/// <param name="out_layers">��Ƭ��ַ</param>
		/// <returns>�Ƿ��ȡ�ɹ�</returns>
		bool getImageLayers( osgEarth::ImageLayerVector& out_layers ) const;

		/// <summary>
		/// ��ȡ��Ƭͼ�����
		/// </summary>
		/// <returns>ͼ�����</returns>
		int getNumImageLayers() const;

		/// <summary>
		/// �������ƻ�ȡ��Ƭͼ�����
		/// </summary>
		/// <param name="name">ͼ������</param>
		/// <returns>ͼ�����</returns>
		osgEarth::ImageLayer* getImageLayerByName( const std::string& name ) const;

		bool getElevationLayers( osgEarth::ElevationLayerVector& out_layers ) const;

		int getNumElevationLayers() const;

		osgEarth::ElevationLayer* getElevationLayerByName( const std::string& name ) const;

		bool getModelLayers( osgEarth::ModelLayerVector& out_layers ) const;

		int getNumModelLayers() const;

		osgEarth::ModelLayer* getModelLayerByName( const std::string& name ) const;

		/// <summary>
		/// �����Ƭͼ��ӿ�
		/// </summary>
		/// <param name="imageUrl">��Ƭ��ַ</param>
		/// <param name="layerName">ͼ������</param>
		/// <param name="mapType">��Ƭ��ͼ����</param>
		/// <param name="minLevel">��С��ʾ����</param>
		/// <param name="maxLevel">�����ʾ����</param>
		/// <returns>ͼ�����</returns>
		osgEarth::ImageLayer* addImageLayer(
			const std::string& imageUrl,
			const std::string& layerName,
			const MapType& mapType,
			const int& minLevel = 0,
			const int& maxLevel = 18);

		void removeImageLayer( osgEarth::ImageLayer* layer );

		osgEarth::ElevationLayer* addElevationLayer(const std::string& elevationUrl,const std::string& layerName);

		void removeElevationLayer( osgEarth::ElevationLayer* layer );

		//ͼ�꣬���ĵ�֧��
		osgEarth::ModelLayer* addPointShpModelLayer(const std::string& shpPath,
			const std::string& layerName,const osgEarth::Symbology::Color& textColor,
			const float& textSize,const std::string& font,const std::string& contextField);

		osgEarth::ModelLayer* addLineShpModelLayer(const std::string& shpPath,const std::string& layerName,
			const osgEarth::Symbology::Color& lineColor,const float& lineWidth);

		//���ڸ߳�ģʽ
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