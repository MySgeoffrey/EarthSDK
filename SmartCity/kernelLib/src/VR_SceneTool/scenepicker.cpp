#include "scenetool/scenepicker.h"
#include <osgFX/Outline>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>
#include <osgViewer/Viewer>
#include <osgManipulator/Dragger>
#include <osgManipulator/CommandManager>
#include <osgManipulator/Selection>
#include <osgManipulator/TrackballDragger>
#include "osgManipulator/Translate1DDragger"
#include "osgManipulator/TabPlaneDragger"
#include "osgManipulator/TabPlaneTrackballDragger"
#include "osgManipulator/TabBoxTrackballDragger"
#include "osgManipulator/TranslateAxisDragger"
#include <osgGA/OrbitManipulator>
#include <osg/ComputeBoundsVisitor>  
#include <osg/PolygonMode> 
#include <osgFX/Scribe>
#include <osgEarth/Picker>
#include <osgEarth/IntersectionPicker>
#include <osgEarth/Registry>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/FeatureNode>

namespace SceneTool
{
	CPickerResult::CPickerResult()
	{
		this->mMouseButton = -1;
		this->mScreenPosition = osg::Vec2f(-1,-1);
		this->mPickerObject = NULL;
	}

	CPickerResult::~CPickerResult()
	{

	}

	osg::Vec2d CPickerResult::getScreenPosition()
	{
		return this->mScreenPosition;
	}

	Common::IObject* CPickerResult::getPickerObject()
	{
		return this->mPickerObject;
	}

	void CPickerResult::setScreenPosition(const osg::Vec2d& screenPosition)
	{
		this->mScreenPosition = screenPosition;
	}

	void CPickerResult::setPickerObject(Common::IObject* pPickerObject)
	{
		this->mPickerObject = pPickerObject;
	}

	void CPickerResult::setMouseButton(const int& mouseButton)
	{
		this->mMouseButton = mouseButton;
	}

	int CPickerResult::getMouseButton()
	{
		return this->mMouseButton;
	}

	struct ModelPickerCallback : public osgEarth::Util::RTTPicker::Callback
	{
	public:
		ModelPickerCallback()
		{
			ModelPickerTool = NULL;
		}

		void onHit(osgEarth::ObjectID id)
		{
			if (this->ModelPickerTool
				&& !this->ModelPickerTool->getEnable())
			{
				return ;
			}
			osgEarth::ObjectIndex* pObjectIndex = osgEarth::Registry::objectIndex();
			Common::IObject* pUserData =
				osgEarth::Registry::objectIndex()->get<Common::IObject>(id);
			if (this->ModelPickerTool)
			{
				this->mPickerResult.setPickerObject(pUserData);
				this->ModelPickerTool->forceListeners(&(this->mPickerResult));
			}
		}
		void onMiss()
		{
			if (this->ModelPickerTool
				&& !this->ModelPickerTool->getEnable())
			{
				return ;
			}
			if (this->ModelPickerTool)
			{
				this->ModelPickerTool->forceListeners(&(this->mPickerResult));
			}
		}
		bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
		{
			if (this->ModelPickerTool
				&& !this->ModelPickerTool->getEnable())
			{
				return false;
			}
			////复位选中对象
			this->mPickerResult.setPickerObject(NULL);

			//设置屏幕坐标
			osg::Vec2f screenPos(ea.getX(),ea.getY());
			this->mPickerResult.setScreenPosition(screenPos);
			this->mPickerResult.setMouseButton(ea.getButton());
			return ea.getEventType() == ea.RELEASE;
		}
	public:
		CModelPickerTool* ModelPickerTool;
		CPickerResult mPickerResult;
	};

	CModelPickerTool::CModelPickerTool(osg::ref_ptr<osgEarth::MapNode> refMapNode)
	{
		this->mRefMapNode = refMapNode;
		this->addChild(this->mRefMapNode);
		ModelPickerCallback* pCallBack = new ModelPickerCallback();
		pCallBack->ModelPickerTool = this;
		this->setDefaultCallback(pCallBack);
		this->mEnable = true;
	}

	CModelPickerTool::~CModelPickerTool()
	{

	}

	bool CModelPickerTool::getEnable()
	{
		return this->mEnable;
	}

	void CModelPickerTool::setEnable(const bool& enable)
	{
		if (this->mEnable == enable)
			return ;
		this->mEnable = enable;
		if (this->mRefMapNode)
		{
			if (this->mEnable)
			{
				this->addChild(this->mRefMapNode);
			}
			else
			{
				this->removeChild(this->mRefMapNode);
			}
		}
	}

	bool CModelPickerTool::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (!this->getEnable())
			return false;
		return osgEarth::Util::RTTPicker::handle(ea,aa);
	}

	void CModelPickerTool::forceListeners(Common::IObject* pResult)
	{
		if (!this->getEnable())
			return ;
		this->forceListenerEvents(pResult);
	}

}

