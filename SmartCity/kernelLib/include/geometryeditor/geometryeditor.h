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
	/// ����Ⱦ����
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IEditPoint
		: public Common::IObject
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IEditPoint();
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IEditPoint();
		/// <summary>
		/// ��ȡ����
		/// </summary>
		virtual int& getIndex();
		/// <summary>
		/// �������� 
		/// </summary>
		virtual void setIndex(const int& index);
		/// <summary>
		/// ��ȡ���������
		/// </summary>
		virtual osg::Vec3d& getGeoPosition();
		/// <summary>
		/// ���õ��������
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
	/// �༭�㼯��
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
	/// ���ζ���༭����
	/// </summary>
	class VR_GEOMETRYEDITOR_DLL IGeometryEditor 
		: public osgGA::GUIEventHandler,
		public osgEarth::Annotation::Dragger::PositionChangedCallback,
		public Tool::ITool
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="pRefMapNode">�����ĵ�ͼ�ڵ�</param>
		IGeometryEditor(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IGeometryEditor();
		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="visible">�����ĵ�ͼ�ڵ�</param>
		virtual void setVisible(const bool& visible);
		/// <summary>
		/// ��ȡ����
		/// </summary>
		virtual bool getVisible();
		/// <summary>
		/// ��ʼ��
		/// </summary>
		virtual void initial();
		/// <summary>
		/// ��ձ༭�ص�
		/// </summary>
		virtual void createEditorCallBack();
		/// <summary>
		/// ���õ�ǰ��׽�ĵ�
		/// </summary>
		virtual void setCurrentPointEnter(IEditPoint* pCurrentPoint);
		/// <summary>
		/// �����༭
		/// </summary>
		virtual bool beginEdit();
		/// <summary>
		///�����༭
		/// </summary>
		virtual bool endEdit(
			const bool& clearCurrentGeometry = true,
			const bool& needSaveEditingResult = true,
			const bool& hideOriginGraphicObject = true);
		/// <summary>
		/// �����Ⱦ��Դ
		/// </summary>
		virtual void clearRenderResource();
		/// <summary>
		/// ��ȡ������Ϣ
		/// </summary>
		std::vector<IEditPoint*>& getGeometry();
		/// <summary>
		/// ��ȡ��ǰ�༭�ĵ㼯��
		/// </summary>
		IEPointSet* getCurrentEditingPointSet();
		/// <summary>
		/// ��ȡ���м���Ҫ��
		/// </summary>
		std::vector<IEPointSet*>& getAllGemetries();
		/// <summary>
		/// ��ȡ��༭���
		/// </summary>
		osgEarth::Symbology::Style& getPointsEditStyle();
		/// <summary>
		/// ��ȡͼ�η��
		/// </summary>
		osgEarth::Symbology::Style& getGeometryStyle();

		void setViewer(osgViewer::Viewer* pRefViewer);
		/// <summary>
		/// �Ƴ���Ⱦ��Դ
		/// </summary>
		virtual void removeRenderResource();
		/// <summary>
		/// ��ȡ�Ƿ�֧�ֲ�ѯ
		/// </summary>
		virtual bool getCanQueryPoint();
		/// <summary>
		/// �����Ƿ�֧�ֲ�ѯ
		/// </summary>
		virtual void setCanQueryPoint(const bool& canQueryPoint);
		/// <summary>
		/// �Ƿ��ڱ༭״̬ 
		/// </summary>
		virtual bool isEditingState();
		/// <summary>
		/// ֪ͨ�仯
		/// </summary>
		virtual void notifyStyleChanged();

		virtual Common::ISceneObject* generateSceneObject();

	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);

		osg::Vec3d getGeoPosition(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);


		virtual void performDoublePicked(osg::Vec3d pos);

		// ��γ����Ϣ
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		// ����������Ϣ
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

