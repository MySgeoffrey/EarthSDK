#ifndef _GEOMETRYEDITOR_H
#define _GEOMETRYEDITOR_H

#include <osgGA/GUIEventHandler>
#include <osgViewer/CompositeViewer>
#include <osgViewer/viewer>  
#include <osgEarth/Viewpoint>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/Common>
#include <osgEarthUtil/GeodeticGraticule>
#include <osgEarth/Cache>
#include <osgEarthUtil/LatLongFormatter>
#include <osgEarthUtil/MouseCoordsTool>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/MapNode>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/FeatureEditing>
#include <osgEarthAnnotation/Draggers>
#include <osgViewer/Viewer>
#include "geometryeditor/geometryeditorconfig.h"
#include "framework/common/isceneobject.h"
#include "framework/tool/itool.h"

namespace GeometryEditor
{
	/// <summary>
	/// 点渲染对象
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IEditPoint
		: public Common::IObject
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IEditPoint();
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IEditPoint();
		/// <summary>
		/// 获取索引
		/// </summary>
		virtual int& getIndex();
		/// <summary>
		/// 设置索引 
		/// </summary>
		virtual void setIndex(const int& index);
		/// <summary>
		/// 获取点地理坐标
		/// </summary>
		virtual osg::Vec3d& getGeoPosition();
		/// <summary>
		/// 设置点地理坐标
		/// </summary>
		virtual void setGeoPosition(const osg::Vec3d& geoPosition);

		virtual void setUserData(osg::ref_ptr<osg::Referenced> pUserData);

		virtual osg::ref_ptr<osg::Referenced>& getUserData();

	protected:
		int mIndex;
		osg::Vec3d mGeoposition;
		osg::ref_ptr<osg::Referenced> mpRefUserData;

	};
	/// <summary>
	/// 编辑点集合
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IEPointSet : public Common::IObject
	{
	public:
		IEPointSet();

		virtual ~IEPointSet();

	public:
		std::vector<IEditPoint*> Data;
	};
	/// <summary>
	/// 几何对象编辑基类
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IGeometryEditor 
		: public osgGA::GUIEventHandler,
		public osgEarth::Annotation::Dragger::PositionChangedCallback,
		public Tool::ITool
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		IGeometryEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IGeometryEditor();
		/// <summary>
		/// 设置显隐
		/// </summary>
		/// <param name="visible">关联的地图节点</param>
		virtual void setVisible(const bool& visible);
		/// <summary>
		/// 获取显隐
		/// </summary>
		virtual bool getVisible();
		/// <summary>
		/// 初始化
		/// </summary>
		virtual void initial();
		/// <summary>
		/// 清空编辑回调
		/// </summary>
		virtual void createEditorCallBack();
		/// <summary>
		/// 设置当前捕捉的点
		/// </summary>
		virtual void setCurrentPointEnter(IEditPoint* pCurrentPoint);
		/// <summary>
		/// 开启编辑
		/// </summary>
		virtual bool beginEdit();
		/// <summary>
		///结束编辑
		/// </summary>
		virtual bool endEdit(
			const bool& clearCurrentGeometry = true,
			const bool& needSaveEditingResult = true,
			const bool& hideOriginGraphicObject = true);
		/// <summary>
		/// 清空渲染资源
		/// </summary>
		virtual void clearRenderResource();
		/// <summary>
		/// 获取几何信息
		/// </summary>
		std::vector<IEditPoint*>& getGeometry();
		/// <summary>
		/// 获取当前编辑的点集合
		/// </summary>
		IEPointSet* getCurrentEditingPointSet();
		/// <summary>
		/// 获取所有几何要素
		/// </summary>
		std::vector<IEPointSet*>& getAllGemetries();
		/// <summary>
		/// 获取点编辑风格
		/// </summary>
		osgEarth::Symbology::Style& getPointsEditStyle();
		/// <summary>
		/// 获取图形风格
		/// </summary>
		osgEarth::Symbology::Style& getGeometryStyle();

		void setViewer(osgViewer::Viewer* pRefViewer);
		/// <summary>
		/// 移除渲染资源
		/// </summary>
		virtual void removeRenderResource();
		/// <summary>
		/// 获取是否支持查询
		/// </summary>
		virtual bool getCanQueryPoint();
		/// <summary>
		/// 设置是否支持查询
		/// </summary>
		virtual void setCanQueryPoint(const bool& canQueryPoint);
		/// <summary>
		/// 是否处于编辑状态 
		/// </summary>
		virtual bool isEditingState();
		/// <summary>
		/// 通知变化
		/// </summary>
		virtual void notifyStyleChanged();

		virtual Common::ISceneObject* generateSceneObject();

	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);

		osg::Vec3d getGeoPosition(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);


		virtual void performDoublePicked(osg::Vec3d pos);

		// 经纬度信息
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		// 世界坐标信息
		virtual void performPickedXYZ(osg::Vec3d pos);

		virtual void performMovingXYZ(osg::Vec3d pos);

		virtual void performRightPicked();

		virtual void performDeleteDrawing();

		virtual bool performMoveGraphic(osg::Vec3d pos);

		virtual bool performEndMoveGraphic(osg::Vec3d pos);

		virtual void onPositionChanged(const osgEarth::Annotation::Dragger* sender, const osgEarth::GeoPoint& position);

	protected:
		bool mCanQueryPoint;
		bool mVisible;
		bool mIsEditState;
		bool mNeedMoveGraphic;
		bool mMouseDownAppendPointMode;
		bool mIsInsertPointState;
		osg::Vec3d mCurGeoPostion;
		osgEarth::MapNode* mpRefMapNode;
		/*std::vector<IEditPoint*> mGeometry;*/
		std::vector<IEPointSet*> mOverEditedGeometrySet;
		IEPointSet* mpCurrentEditingGeometry;
		osgEarth::Symbology::Style mGeometryStyle;
		osgEarth::Symbology::Style mPointsEditStyle;
		osg::ref_ptr<osgEarth::Annotation::FeatureNode> mpGeometryNode;
		osg::ref_ptr<osgEarth::Annotation::FeatureEditor> mpPointsEdit;
		osgViewer::Viewer* mpRefViewer;
		IEditPoint* mpRefCurrentPoint;
		bool mIsDragState;
		int mPreInsertPointIndex;
		int mNextInsertPointIndex;
	};

	class VR_GEOMETRYEDITOR_DLL SphereDraggerEx
		: public osgEarth::Annotation::SphereDragger
	{
	public:
		SphereDraggerEx(osgEarth::MapNode* mapNode);

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		virtual void enter();

		virtual void leave();

		virtual void setNeedHandleGUIEvent(const bool& needHandleGUIEvent);

		virtual bool getNeedHandleGUIEvent();

	public:
		IEditPoint* mpRefOwnerPoint;
		IGeometryEditor* mpRefOwnerEditor;
		bool mNeedHandleGUIEvent;
	};
}

#endif//_GEOMETRYEDITOR_H

