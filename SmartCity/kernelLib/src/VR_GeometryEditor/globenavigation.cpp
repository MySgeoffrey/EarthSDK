#include "globenavigation.h"
#include "framework/common/globesetting.h"
#include <osgEarth/ElevationQuery>

osg::BoundingBox g_scaleBox;
osg::BoundingBox g_disBox;

bool getWorldCoordsUnderMouse(osg::View* view, float x, float y, osg::Vec3d& out_coords )
{
    osgViewer::View* view2 = dynamic_cast<osgViewer::View*>(view);
    if ( !view2 )
        return false;

    // New code, uses the code from osg::View::computeIntersections but uses our DPLineSegmentIntersector instead to get around floating point precision issues.
    float local_x, local_y = 0.0;
    const osg::Camera* camera = view2->getCameraContainingPosition(x, y, local_x, local_y);
    if (!camera) camera = view2->getCamera();

    osg::Matrixd matrix;
    matrix.postMult(camera->getViewMatrix());
    matrix.postMult(camera->getProjectionMatrix());

    double zNear = -1.0;
    double zFar = 1.0;
    if (camera->getViewport())
    {
        matrix.postMult(camera->getViewport()->computeWindowMatrix());
        zNear = 0.0;
        zFar = 1.0;
    }

    osg::Matrixd inverse;
    inverse.invert(matrix);

    out_coords = osg::Vec3d(x,y,zNear) * inverse;
	return true;
}


class MouseCoordsCallback : public MouseCoordsTool::Callback
{
public:
	MouseCoordsCallback(CGlobeNavigation* pOwner)
	{
		this->mpOwner = pOwner;
	}

	virtual ~MouseCoordsCallback() { }

	virtual void set(const GeoPoint& coords, osg::View* view, MapNode* mapNode)
	{
		if (this->mpOwner)
		{
			double demAlt = 0;
			osg::ref_ptr<osgEarth::MapNode> pMapNode = this->mpOwner->mpRefGlobeWidgetEx->getMapNodeRef();
			osg::ref_ptr<osgEarth::Map> pMap = this->mpOwner->mpRefGlobeWidgetEx->getMapRef();
			//精度低
			//pMapNode->getTerrain()->getHeight(this->mpOwner->mpRefGlobeWidgetEx->getMapNodeRef()->getMapSRS(),coords.x(), coords.y(), &demAlt);
			//精度高
			double query_resolution = 0.1;			
			double out_resolution = 0.0;
			osgEarth::ElevationQuery query(pMap.get());
			query.getElevation(GeoPoint(pMapNode->getMapSRS(), coords.x(), coords.y(), 0.0, osgEarth::AltitudeMode::ALTMODE_RELATIVE),
				demAlt,
				query_resolution,
				&out_resolution);

			osg::Vec3d eye, up, center;
			view->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			double distanceToGround = 0;
			double xxlon = 0.0, yylan = 0.0;// eye.length();
			const osg::EllipsoidModel* pellipsoidmodel = pMapNode->getMapSRS()->getEllipsoid();
			pellipsoidmodel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
				yylan, xxlon, distanceToGround);
			xxlon = osg::RadiansToDegrees(xxlon);
			yylan = osg::RadiansToDegrees(yylan);

			if (this->mpOwner->getStateInfoVisible())
			{
				std::string lonStr;
				std::string latStr;
				std::string altStr;
				std::string cameraStr;

				CGlobeSetting::instance()->gb2312ToUtf8("经度：", lonStr);
				CGlobeSetting::instance()->gb2312ToUtf8("纬度：", latStr);
				CGlobeSetting::instance()->gb2312ToUtf8("高程：", altStr);
				CGlobeSetting::instance()->gb2312ToUtf8("视点高度：", cameraStr);
				_dAltitude = distanceToGround;
				std::stringstream stateText;
#if 0
				stateText << lonStr << do_fraction(coords.x(), 7) << "  " 
					<< latStr << do_fraction(coords.y(), 7) << "  " 
					<< altStr << do_fraction(demAlt, 3) << "  " 
					<< cameraStr << do_fraction(distanceToGround, 3);
#else
				std::string lonStrEx = this->mpOwner->mLongitudeVisible ? (lonStr + do_fraction(coords.x(), 7) + "  " ) : "";
				std::string latStrEx = this->mpOwner->mLantitudeVisible ? (latStr + do_fraction(coords.y(), 7) + "  " ) : "";
				std::string demStrEx = this->mpOwner->mTerrainAltitudeVisible ? (altStr + do_fraction(demAlt, 7) + "  " ) : "";
				std::string viewStrEx = this->mpOwner->mViewAltitudeVisible ? (cameraStr + do_fraction(distanceToGround, 7) + "  " ) : "";
				stateText << lonStrEx << latStrEx << demStrEx << viewStrEx;
#endif
				this->mpOwner->mStateLabelText->setText(stateText.str(), osgText::String::ENCODING_UTF8);
			}
	
			/*if (this->mpOwner->getEagleVisible())
			{
				double lonRatio = (xxlon - (-180)) / (double)360;
				double latRatio = (90 - yylan) / (double)180;
				int relativeX = (int)(this->mpOwner->mEagleImageControl->width().get() * lonRatio);
				int relativeY = (int)(this->mpOwner->mEagleImageControl->height().get() * latRatio);
				float pX = this->mpOwner->mEagleImageControl->x().get() + relativeX;
				float pY = this->mpOwner->mEagleImageControl->y().get() + relativeY;
				this->mpOwner->mEaglePositionControl->setPosition(pX, pY);
			}*/

			//if (this->mpOwner->getGlobeScaleVisible())
			//{
			//	osg::Vec3d startWorldPoint,endWorldPoint;
			//	getWorldCoordsUnderMouse(view,0,0,startWorldPoint);
			//	getWorldCoordsUnderMouse(view,100,0,endWorldPoint);
			//	double realDis = (endWorldPoint - startWorldPoint).length();
			//	double mapDis = 100 * 0.3 /1000.0;
			//	double scale = mapDis / realDis;
			//	
			//	//比例尺图片长度代表实际的距离
			//	double imageRealDis = 78 * 0.3 / (scale * 1000.0);
			//	char imageRealDisStr[200];
			//	if (imageRealDis > 1000)
			//	{
			//		imageRealDis /= 1000.0;
			//		sprintf(imageRealDisStr, "%.0fkm", imageRealDis);
			//	}
			//	else
			//	{
			//		sprintf(imageRealDisStr, "%.0fm", imageRealDis);
			//	}
			//	
			//	char scaleStr[200];
			//	sprintf(scaleStr, "1:%.0f", 1 / scale);
			//	this->mpOwner->mGlobeScaleText->setText(scaleStr, osgText::String::ENCODING_UTF8);
			//	this->mpOwner->mScaleDisText->setText(imageRealDisStr, osgText::String::ENCODING_UTF8);
			//	this->mpOwner->updateGlobeScaleInfo();
			//}
		
		}
	}
	virtual void reset(osg::View* view, MapNode* mapNode)
	{

	}
	inline double getCurrentAltitude()
	{
		return _dAltitude;
	}
	string do_fraction(long double value, int decplaces = 3)
	{
		ostringstream out;
		int prec =
			std::numeric_limits<double>::digits10; // 18
		out.precision(prec);//覆盖默认精度
		out << value;
		string str = out.str(); //从流中取出字符串
		char DECIMAL_POINT = '.';
		size_t n = str.find(DECIMAL_POINT);
		if ((n != string::npos) //有小数点吗？
			&& (str.size() > n + decplaces)) //后面至少还有decplaces位吗？
		{
			str[n + decplaces] = '\0';//覆盖第一个多余的数
		}

		str.swap(string(str.c_str()));//删除nul之后的多余字符

		return str;
	}

public:
	CGlobeNavigation* mpOwner;
	double _dAltitude;
};

class CCameraCallBack : public osg::NodeCallback
{
public:
	CCameraCallBack()
	{
		this->mpOwner = NULL;
	}

	~CCameraCallBack()
	{
	
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if (this->mpOwner)
		{
			double demAlt = 0;
			osg::ref_ptr<osgEarth::MapNode> pMapNode = this->mpOwner->mpRefGlobeWidgetEx->getMapNodeRef();
			osg::ref_ptr<osgEarth::Map> pMap = this->mpOwner->mpRefGlobeWidgetEx->getMapRef();

			osg::Vec3d eye, up, center;
			this->mpOwner->mpRefGlobeWidgetEx->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			double distanceToGround = 0;
			double xxlon = 0.0, yylan = 0.0;// eye.length();
			const osg::EllipsoidModel* pellipsoidmodel = pMapNode->getMapSRS()->getEllipsoid();
			pellipsoidmodel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
				yylan, xxlon, distanceToGround);
			xxlon = osg::RadiansToDegrees(xxlon);
			yylan = osg::RadiansToDegrees(yylan);

			if (this->mpOwner->getEagleVisible())
			{
				double lonRatio = (xxlon - (-180)) / (double)360;
				double latRatio = (90 - yylan) / (double)180;
				int relativeX = (int)(this->mpOwner->mEagleImageControl->width().get() * lonRatio);
				int relativeY = (int)(this->mpOwner->mEagleImageControl->height().get() * latRatio);
				float pX = this->mpOwner->mEagleImageControl->x().get() + relativeX;
				float pY = this->mpOwner->mEagleImageControl->y().get() + relativeY;
				this->mpOwner->mEaglePositionControl->setPosition(pX, pY);
			}

			if (this->mpOwner->getGlobeScaleVisible())
			{
				osg::Vec3d startWorldPoint,endWorldPoint;
				getWorldCoordsUnderMouse((osgViewer::Viewer*)this->mpOwner->mpRefGlobeWidgetEx->getViewer(),0,0,startWorldPoint);
				getWorldCoordsUnderMouse((osgViewer::Viewer*)this->mpOwner->mpRefGlobeWidgetEx->getViewer(),100,0,endWorldPoint);
				double realDis = (endWorldPoint - startWorldPoint).length();
				double mapDis = 100 * 0.3 /1000.0;
				double scale = mapDis / realDis;
				
				//比例尺图片长度代表实际的距离
				double imageRealDis = 78 * 0.3 / (scale * 1000.0);
				char imageRealDisStr[200];
				if (imageRealDis > 1000)
				{
					imageRealDis /= 1000.0;
					sprintf(imageRealDisStr, "%.0fkm", imageRealDis);
				}
				else
				{
					sprintf(imageRealDisStr, "%.0fm", imageRealDis);
				}
				
				char scaleStr[200];
				sprintf(scaleStr, "1:%.0f", 1 / scale);
				this->mpOwner->mGlobeScaleText->setText(scaleStr, osgText::String::ENCODING_UTF8);
				this->mpOwner->mScaleDisText->setText(imageRealDisStr, osgText::String::ENCODING_UTF8);
				this->mpOwner->updateGlobeScaleInfo();
			}
		
		}
	}

public:
	CGlobeNavigation* mpOwner;
};

osg::Camera* CGlobeNavigation::createOrthoCamera(double width, double height)
{
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
		);

	osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);

	camera->setProjectionMatrix(m);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT); 
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	return camera;
}

void CGlobeNavigation::textInfo(osgText::Text* text)
{
	const osgText::Text::TextureGlyphQuadMap& tgqm = text->getTextureGlyphQuadMap();

	const osgText::Text::TextureGlyphQuadMap::const_iterator tgqmi = tgqm.begin();



	const osgText::Text::GlyphQuads& gq = tgqmi->second;

	osgText::String& s = text->getText();

	for (unsigned int i = 0; i < s.size(); i++)
	{
		osg::Vec2 ul = (*gq.getCoords())[0 + (i * 4)]; // upperLeft
		osg::Vec2 ll = (*gq.getCoords())[1 + (i * 4)]; // lowerLeft
		osg::Vec2 lr = (*gq.getCoords())[2 + (i * 4)]; // lowerRight
		osg::Vec2 ur = (*gq.getCoords())[3 + (i * 4)]; // upperRight

	}
}

osgText::Text* CGlobeNavigation::createLabel(const std::string& l, const char* f, unsigned int size)
{
	static osg::Vec3 pos(100.0f, 150.0f, 0.0f);
	//static osg::Vec3 pos(8000, 100, 0.0f);

	osgText::Text* label = new osgText::Text();
	osgText::Font* font = osgText::readFontFile("fonts/simhei.ttf");
	
	label->setFont(font);
	label->setCharacterSize(size);
	label->setFontResolution(size, size);
	label->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	label->setPosition(pos);
	label->setAlignment(osgText::Text::LEFT_BOTTOM);

	// It seems to be important we do this last to get best results?
	label->setText(l, osgText::String::ENCODING_UTF8);

	textInfo(label);

	pos.y() += size + 10.0f;
	//g_label = label;
	return label;
}


CGlobeNavigation::CGlobeNavigation(VRGlobeWidget* pRefGlobeWidgetEx)
	: mpRefGlobeWidgetEx(pRefGlobeWidgetEx), mCompassControl(NULL),
	mLevelControl(NULL), mLevelSelectControl(NULL),
	mCameraMoveUpControl(NULL), mCameraMoveDownControl(NULL),
	mCameraMoveLeftControl(NULL), mCameraMoveRightControl(NULL),
	mStateInfoControl(NULL), mCanSelectLevel(false), mMouseDownX(-1), mMouseDownY(-1), mStateInfoVisible(true), mNavigationVisible(true),
	mX(50), mY(30), mHasInitialised(false), mMouseButton(1), mLevelButtonX(-1), mLevelButtonY(-1),
	mEagleVisible(false), mEagleImageControl(NULL), mEaglePositionControl(NULL)
{
	this->mGlobeScalePositionX = 10;
	this->mGlobeScalePositionY = 50;
	this->mGlobeScaleVisible = true;
}


CGlobeNavigation::~CGlobeNavigation()
{
	this->mpRefGlobeWidgetEx->removeEventHandler(this);
	//mRadarCamera->unref();
}

void CGlobeNavigation::setStateTextMode(const bool& longitudeVisible,const bool& lantitudeVisible,
	const bool& terrainAltitudeVisible,const bool& viewAltitudeVisible)
{
	this->mLongitudeVisible = longitudeVisible;
	this->mLantitudeVisible = lantitudeVisible;
	this->mTerrainAltitudeVisible = terrainAltitudeVisible;
	this->mViewAltitudeVisible = viewAltitudeVisible;
}

void CGlobeNavigation::getStateTextMode(bool& longitudeVisible,bool& lantitudeVisible,
	bool& terrainAltitudeVisible,bool& viewAltitudeVisible)
{
	longitudeVisible = this->mLongitudeVisible;
	lantitudeVisible = this->mLantitudeVisible;
	terrainAltitudeVisible = this->mTerrainAltitudeVisible;
	viewAltitudeVisible = this->mViewAltitudeVisible;
}


void CGlobeNavigation::updateCompass()
{
	if (this->mpRefGlobeWidgetEx)
	{
		osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManip =
			dynamic_cast<osgEarth::Util::EarthManipulator*>(this->mpRefGlobeWidgetEx->getCameraManipulator());
		if (earthManip)
		{
			double angle = earthManip->getViewpoint().heading().get().getValue();
			angle = -1 * angle;
			this->mCompassControl->setRotation(angle);
		}
	}

}

bool CGlobeNavigation::getNavigationVisible()
{
	return this->mNavigationVisible;
}

void CGlobeNavigation::setNavigationVisible(const bool& visible)
{
	this->mNavigationVisible = visible;
	if (this->mCompassControl)
	{
		this->mCompassControl->setVisible(visible);
		this->mLevelControl->setVisible(visible);
		this->mLevelSelectControl->setVisible(visible);
		this->mCameraMoveUpControl->setVisible(visible);
		this->mCameraMoveDownControl->setVisible(visible);
		this->mCameraMoveLeftControl->setVisible(visible);
		this->mCameraMoveRightControl->setVisible(visible);
	}
	
}

bool CGlobeNavigation::getStateInfoVisible()
{
	return this->mStateInfoVisible;
}

void CGlobeNavigation::setStateInfoVisible(const bool& visible)
{
	this->mStateInfoVisible = visible;
	if (this->mLabelCamera)
	{
		osg::Node* pTextNode = this->mLabelCamera;
		if (visible)
		{
			pTextNode->setNodeMask(1);
		}
		else
		{
			pTextNode->setNodeMask(0);
		}
	}

	if (this->mStateInfoControl)
	{
		this->mStateInfoControl->setVisible(visible);
	}
}

bool CGlobeNavigation::getEagleVisible()
{
	return this->mEagleVisible;
}

void CGlobeNavigation::setEagleVisible(const bool& visible)
{
	this->mEagleVisible = visible;
	this->mEagleImageControl->setVisible(this->mEagleVisible);
	this->mEaglePositionControl->setVisible(this->mEagleVisible);
	if (this->getEagleVisible())
	{
		this->mEagleImageControl->setPosition(this->mpRefGlobeWidgetEx->width() - 270, 
			this->mpRefGlobeWidgetEx->height() - 140);		
		osg::Vec3d eye, up, center;
		this->mpRefGlobeWidgetEx->getCamera()->getViewMatrixAsLookAt(eye, center, up);
		double distanceToGround = 0;
		double xxlon = 0.0, yylan = 0.0;// eye.length();
		const osg::EllipsoidModel* pellipsoidmodel = this->mpRefGlobeWidgetEx->getMapNodeRef()->getMapSRS()->getEllipsoid();
		pellipsoidmodel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
			yylan, xxlon, distanceToGround);
		xxlon = osg::RadiansToDegrees(xxlon);
		yylan = osg::RadiansToDegrees(yylan);

#if 1
		double lonRatio = (xxlon - (-180)) / (double)360;
		double latRatio = (90 - yylan) / (double)180;
		int relativeX = (int)(this->mEagleImageControl->width().get() * lonRatio);
		int relativeY = (int)(this->mEagleImageControl->height().get() * latRatio);
		float pX = this->mEagleImageControl->x().get() + relativeX;
		float pY = this->mEagleImageControl->y().get() + relativeY;
		this->mEaglePositionControl->setPosition(pX, pY);
#endif
	}
}


void CGlobeNavigation::inital()
{
	ControlCanvas* canvas = new ControlCanvas();

	canvas->setNodeMask( 0x1 << 1 );
	CHAR ch[MAX_PATH]; 
	CHAR longPath[MAX_PATH];  
#if QT_NO_DEBUG
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidget.dll"), ch, MAX_PATH);  // 使用Windows API 获取到ActiveX控件在磁盘上的绝对路径 
#else
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidgetd.dll"), ch, MAX_PATH);  // 使用Windows API 获取到ActiveX控件在磁盘上的绝对路径 
#endif 
	GetLongPathNameA(ch,longPath,MAX_PATH);  
	
	//初始化创建指南针导航键
	std::string compassControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/compass_n.png";
	mCompassControl = new ImageControl();
	osg::Image* pCompassControlTexture = osgDB::readImageFile(compassControlTexturePath);
	mCompassControl->setImage(pCompassControlTexture);
	//mCompassControl->setMargin(10);
	/*Control::Alignment ha = mCompassControl->horizAlign().get();
	Control::Alignment va = mCompassControl->vertAlign().get();
	mCompassControl->setHorizAlign(Control::Alignment::ALIGN_CENTER);
	mCompassControl->setVertAlign(Control::Alignment::ALIGN_CENTER);
	mCompassControl->setRotation(45);*/
	mCompassControl->setFixSizeForRotation(true);

	
	//初始化创建级别导航键
	std::string levelControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/compass.png";
	mLevelControl = new ImageControl();
	osg::Image* pLevelControlTexture = osgDB::readImageFile(levelControlTexturePath);
	mLevelControl->setImage(pLevelControlTexture);
	mLevelControl->setMargin(10);

	//初始化创建级别选择导航键
	std::string levelSelectControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/silder_compass.png";
	mLevelSelectControl = new ImageControl();
	osg::Image* pLevelSelectControlTexture = osgDB::readImageFile(levelSelectControlTexturePath);
	mLevelSelectControl->setImage(pLevelSelectControlTexture);
	mLevelSelectControl->setMargin(10);

	//初始化创建相机上移导航键
	std::string moveUpControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/up_compass.png";
	mCameraMoveUpControl = new ImageControl();
	osg::Image* pMoveUpControlTexture = osgDB::readImageFile(moveUpControlTexturePath);
	mCameraMoveUpControl->setImage(pMoveUpControlTexture);
	mCameraMoveUpControl->setMargin(10);
	mCameraMoveUpControl->setVisible(false);

	//初始化创建相机下移导航键
	std::string moveDownControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/down_compass.png";
	mCameraMoveDownControl = new ImageControl();
	osg::Image* pMoveDownControlTexture = osgDB::readImageFile(moveDownControlTexturePath);
	mCameraMoveDownControl->setImage(pMoveDownControlTexture);
	mCameraMoveDownControl->setMargin(10);
	mCameraMoveDownControl->setVisible(false);

	//初始化创建相机左移导航键
	std::string moveLeftControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/left_compass.png";
	mCameraMoveLeftControl = new ImageControl();
	osg::Image* pMoveLeftControlTexture = osgDB::readImageFile(moveLeftControlTexturePath);
	mCameraMoveLeftControl->setImage(pMoveLeftControlTexture);
	mCameraMoveLeftControl->setMargin(10);
	mCameraMoveLeftControl->setVisible(false);

	//初始化创建相机右移导航键
	std::string moveRightControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/right_compass.png";
	mCameraMoveRightControl = new ImageControl();
	osg::Image* pMoveRightControlTexture = osgDB::readImageFile(moveRightControlTexturePath);
	mCameraMoveRightControl->setImage(pMoveRightControlTexture);
	mCameraMoveRightControl->setMargin(10);
	mCameraMoveRightControl->setVisible(false);

	//初始化EyeDisable导航键
	std::string eyeDisableControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/disable_eye.png";
	mEyeDisableControl = new ImageControl();
	osg::Image* pEyeDisableControlTexture = osgDB::readImageFile(eyeDisableControlTexturePath);
	mEyeDisableControl->setImage(pEyeDisableControlTexture);
	mEyeDisableControl->setMargin(10);
	mEyeDisableControl->setVisible(true);

	//初始化EyeView导航键
	std::string eyeViewControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/eye_view.png";
	mEyeViewControl = new ImageControl();
	osg::Image* pEyeViewControlTexture = osgDB::readImageFile(eyeViewControlTexturePath);
	mEyeViewControl->setImage(pEyeViewControlTexture);
	mEyeViewControl->setMargin(10);
	mEyeViewControl->setVisible(false);

	//Angular angle(45);
	//mCompassControl->setRotation(angle);

	//canvas->addControl( mCompassControl );
	canvas->addControl( mLevelControl );
	canvas->addControl( mLevelSelectControl );
	canvas->addControl( mCameraMoveUpControl );
	canvas->addControl( mCameraMoveDownControl );
	canvas->addControl( mCameraMoveLeftControl );
	canvas->addControl( mCameraMoveRightControl );
	canvas->addControl(mEyeDisableControl);
	canvas->addControl(mEyeViewControl);

	
	mStateInfoControl = new Grid();
	mStateInfoControl->setBackColor(0.5, 0.5, 0.5, 0.1);
	mStateInfoControl->setMargin(0);
	mStateInfoControl->setPadding(0);
	mStateInfoControl->setChildSpacing(0);
	mStateInfoControl->setPosition(0, 700);
	mStateInfoControl->setWidth(700);
	mStateInfoControl->setHeight(50);
	mStateInfoControl->setChildVertAlign(Control::ALIGN_CENTER);
	mStateInfoControl->setAbsorbEvents(true);
	mStateInfoControl->setVertAlign(Control::ALIGN_BOTTOM);

	ControlCanvas* eagleCanvas = new ControlCanvas();
	std::string eagleControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/earth.jpg";
	mEagleImageControl = new ImageControl();
	osg::Image* pEagleControlTexture = osgDB::readImageFile(eagleControlTexturePath);
	mEagleImageControl->setImage(pEagleControlTexture);
	mEagleImageControl->setMargin(0);
	mEagleImageControl->setHeight(128);
	mEagleImageControl->setWidth(256);
	mEagleImageControl->setVisible(false);
	eagleCanvas->addControl(mEagleImageControl);

	std::string eaglePositionControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/ealgePostion.png";
	mEaglePositionControl = new ImageControl();
	osg::Image* pEaglePositionControlTexture = osgDB::readImageFile(eaglePositionControlTexturePath);
	mEaglePositionControl->setImage(pEaglePositionControlTexture);
	mEaglePositionControl->setMargin(0);
	mEaglePositionControl->setHeight(8);
	mEaglePositionControl->setWidth(8);
	mEaglePositionControl->setVisible(false);
	eagleCanvas->addControl(mEaglePositionControl);

	//mGlobeScaleControl
	std::string globeScaleControlTexturePath = osgDB::getFilePath(longPath) + "/systemImages/navigation/globeScale.png";
	mGlobeScaleControl = new ImageControl();
	osg::Image* pGlobeScaleControlTexture = osgDB::readImageFile(globeScaleControlTexturePath);
	mGlobeScaleControl->setImage(pGlobeScaleControlTexture);
	mGlobeScaleControl->setMargin(0);
	mGlobeScaleControl->setVisible(true);
	eagleCanvas->addControl(this->mGlobeScaleControl);

	canvas->addControl(mStateInfoControl);
	canvas->addControl(mCompassControl);
	this->mpRefGlobeWidgetEx->getRootRef()->addChild( canvas );
	this->mpRefGlobeWidgetEx->getRootRef()->addChild(eagleCanvas);
	this->mpRefGlobeWidgetEx->addEventHandler(this);


	//初始化状态栏信息
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	std::string lonStr = "Lon:";
	std::string latStr = "Lat:";
	std::string altStr = "Alt:";
	/*CCommonTool::gb2312ToUtf8(lonStr, lonStr);
	CCommonTool::gb2312ToUtf8(latStr, latStr);
	CCommonTool::gb2312ToUtf8(altStr, altStr);*/
	std::stringstream ss;
	ss << "none";
	this->mStateLabelText = createLabel(ss.str(), "", 14);
	this->mStateLabelText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
	this->mStateLabelText->setBackdropColor(osg::Vec4(0,0,0,1));
	this->mStateLabelText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	geode->addDrawable(this->mStateLabelText);
	
	this->mGlobeScaleText = createLabel("1:500000", "", 14);
	this->mGlobeScaleText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
	this->mGlobeScaleText->setBackdropColor(osg::Vec4(0,0,0,1));
	this->mGlobeScaleText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	geode->addDrawable(this->mGlobeScaleText);

	this->mScaleDisText = createLabel("50000km", "", 14);
	this->mScaleDisText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
	this->mScaleDisText->setBackdropColor(osg::Vec4(0,0,0,1));
	this->mScaleDisText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	geode->addDrawable(this->mScaleDisText);
	g_scaleBox = osgEarth::Utils::getBoundingBox(this->mGlobeScaleText);
	g_disBox = osgEarth::Utils::getBoundingBox(this->mScaleDisText);
	const static int width = 1280, height = 1024;
	this->mLabelCamera = createOrthoCamera(width, height);
	this->mLabelCamera->addChild(geode);
	this->mLabelCamera->setViewport(0, 0, width, height);
	//this->mpRefGlobeWidgetEx->getMapNodeRef()->addChild(this->mLabelCamera);
	osg::Group* root = this->mpRefGlobeWidgetEx->getMapNodeRef()->getNumParents() ?
		this->mpRefGlobeWidgetEx->getMapNodeRef()->getParent(0) : NULL;
	root ? root->addChild(this->mLabelCamera) : this->mpRefGlobeWidgetEx->getMapNodeRef()->addChild(this->mLabelCamera);

	mMouseCoordsCallback = new MouseCoordsCallback(this);
	this->mpRefGlobeWidgetEx->getMouseCoordsTool()->addCallback(mMouseCoordsCallback);

	mCameraCallBack = new CCameraCallBack();
	mCameraCallBack->mpOwner = this;
	this->mpRefGlobeWidgetEx->getCamera()->addUpdateCallback(mCameraCallBack);
	
	//createEagleCamera();

	mHasInitialised = true;
}

//void CGlobeNavigation::createEagleCamera()
//{
//	mRadarCamera = new osg::Camera;
//	mRadarCamera->ref();
//	mRadarCamera->setName("radar");
//
//	mRadarCamera->setClearColor(osg::Vec4(0.02, 0.02, 0.02, 1.0));
//	mRadarCamera->setRenderOrder(osg::Camera::POST_RENDER);
//	mRadarCamera->setAllowEventFocus(false);
//	mRadarCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	mRadarCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//	//radar->setViewMatrix(osg::Matrix::identity());
//	
//	mRadarCamera->setViewport(0, 30, 256, 256);
//	mRadarCamera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(256) / static_cast<double>(256), 1.0, 10000.0);	
//
//	mRadarCamera->addChild(this->mpRefGlobeWidgetEx->getMapNodeRef());
//
//	this->mpRefGlobeWidgetEx->getRootRef()->addChild(this->mRadarCamera);
//
//}

void CGlobeNavigation::updateGlobeScaleInfo()
{
	if (this->getGlobeScaleVisible())
	{
		double height = this->mpRefGlobeWidgetEx->height();
		g_scaleBox = osgEarth::Utils::getBoundingBox(this->mGlobeScaleText);
		g_disBox = osgEarth::Utils::getBoundingBox(this->mScaleDisText);
		//比例尺
		this->mGlobeScaleControl->setPosition(
			this->mGlobeScalePositionX, height - this->mGlobeScalePositionY);
		double imageWidth = this->mGlobeScaleControl->getImage()->s();
		double imageHeight = this->mGlobeScaleControl->getImage()->t();
		double textWidth = g_scaleBox.xMax() - g_scaleBox.xMin();
		double disWidth = g_disBox.xMax() - g_disBox.xMin();
		this->mScaleDisText->setPosition(
			osg::Vec3(this->mGlobeScalePositionX + (imageWidth - disWidth) / 2, 
			this->mGlobeScalePositionY, 0.0));
		this->mGlobeScaleText->setPosition(
			osg::Vec3(this->mGlobeScalePositionX + (imageWidth - textWidth) / 2, 
			this->mGlobeScalePositionY- 2.5 * imageHeight, 0.0));
	}
}

void CGlobeNavigation::resize(const int& width,const int& height)
{
	if (mHasInitialised)
	{
		if (this->getNavigationVisible())
		{
#if 1
			this->mCompassControl->setPosition(this->mX - 8, this->mY - 13);
			this->mCompassControl->setMargin(0);
			this->mCompassControl->setPadding(0);
			this->mCompassControl->setRotation(0);
			this->mCompassControl->setFixSizeForRotation(true);
			this->updateCompass();
#else
			this->mCompassControl->setPosition(this->mX + 15, this->mY + 10);
#endif
			this->mLevelControl->setPosition(this->mX + 5, this->mY - 3);
			this->mLevelSelectControl->setPosition(this->mX + 29, this->mY + 140);
			this->mCameraMoveUpControl->setPosition(this->mX + 33, this->mY + 15);
			this->mCameraMoveDownControl->setPosition(this->mX + 33, this->mY + 40);
			this->mCameraMoveLeftControl->setPosition(this->mX + 19, this->mY + 28);
			this->mCameraMoveRightControl->setPosition(this->mX + 46, this->mY + 28);
			this->mEyeDisableControl->setPosition(this->mX + 32, this->mY + 28);
			this->mEyeViewControl->setPosition(this->mX + 32, this->mY + 28);

		}

		this->updateGlobeScaleInfo();
		
		if (this->getStateInfoVisible())
		{
			this->mStateLabelText->setPosition(osg::Vec3(30, 3, 0.0));
			/*osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);
			mLabelCamera->setProjectionMatrix(m);*/

			float fontSize = this->mStateLabelText->getCharacterHeight();
			fontSize += 10;
			this->mStateInfoControl->setPosition(0, height - fontSize);
			this->mStateInfoControl->setWidth(width);
			this->mStateInfoControl->setHeight(fontSize);
		}

		if (this->getEagleVisible())
		{
			this->mEagleImageControl->setPosition(this->mpRefGlobeWidgetEx->width() - 270,
				this->mpRefGlobeWidgetEx->height() - 140);
			osg::Vec3d eye, up, center;
			this->mpRefGlobeWidgetEx->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			double distanceToGround = 0;
			double xxlon = 0.0, yylan = 0.0;// eye.length();
			const osg::EllipsoidModel* pellipsoidmodel = this->mpRefGlobeWidgetEx->getMapNodeRef()->getMapSRS()->getEllipsoid();
			pellipsoidmodel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
				yylan, xxlon, distanceToGround);
			xxlon = osg::RadiansToDegrees(xxlon);
			yylan = osg::RadiansToDegrees(yylan);

#if 1
			double lonRatio = (xxlon - (-180)) / (double)360;
			double latRatio = (90 - yylan) / (double)180;
			int relativeX = (int)(this->mEagleImageControl->width().get() * lonRatio);
			int relativeY = (int)(this->mEagleImageControl->height().get() * latRatio);
			float pX = this->mEagleImageControl->x().get() + relativeX;
			float pY = this->mEagleImageControl->y().get() + relativeY;
			this->mEaglePositionControl->setPosition(pX, pY);
#endif
		}
		if (this->mpRefGlobeWidgetEx)
		{
			this->mpRefGlobeWidgetEx->getViewer()->frame();
		}
	}
}

bool CGlobeNavigation::contains(ImageControl* pControl, float dy, int x, int y)
{
	bool r = false;
	float xc = pControl->x().get() + pControl->margin().offset().x();
	//osg的屏幕坐标系统与imageControl的二维坐标系Y轴方向相反
	float yc = this->mpRefGlobeWidgetEx->height() - (dy + pControl->y().get() + pControl->margin().offset().x());
	float widthC = pControl->getImage()->s();
	float heightC = pControl->getImage()->t() - dy;
	if (x >= xc && x <= xc + widthC
		&& y <= yc && y >= yc - heightC + 15)
	{
		r = true;
	}
	return r;
}

bool CGlobeNavigation::contains(ImageControl* pControl, int x, int y)
{
	if (!pControl->getImage())
		return false;
	bool r = false;
	float xc = pControl->x().get() + pControl->margin().offset().x();
	//osg的屏幕坐标系统与imageControl的二维坐标系Y轴方向相反
	float yc = this->mpRefGlobeWidgetEx->height() - (pControl->y().get() + pControl->margin().offset().x());
	float widthC = pControl->getImage()->s();
	float heightC = pControl->getImage()->t();
	if (x >= xc && x <= xc + widthC
		&& y <= yc && y >= yc - heightC)
	{
		r = true;
	}
	return r;
}

bool CGlobeNavigation::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	bool r = false;
	/*osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(aa.asView());
	if (viewer && mRadarCamera)
	{
	osg::Vec3 eye, center, up;
	double lat, lon, hei;
	viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
	mpRefGlobeWidgetEx->getMapRef()->getProfile()->getSRS()->getEllipsoid()->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(), lat, lon, hei);

	osg::Matrix mts;
	mpRefGlobeWidgetEx->getMapRef()->getProfile()->getSRS()->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(lat, lon, hei + 1500000, mts);
	osg::Matrix mts2;
	mts2 = osg::Matrixd::inverse(mts);
	mRadarCamera->setViewMatrix(mts2);
	}*/
	if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == mMouseButton)
	{
		int mouseDownX = ea.getX();
		int mouseDownY = ea.getY();
		this->mMouseDownX = mouseDownX;
		this->mMouseDownY = mouseDownY;
		this->mCanSelectLevel = this->contains(this->mLevelSelectControl, mouseDownX, mouseDownY);
		if (this->mCanSelectLevel)
		{
			this->mLevelButtonX = this->mLevelSelectControl->x().get();
			this->mLevelButtonY = this->mLevelSelectControl->y().get();
			r = true;
			ea.setHandled(true);
		}

		osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManip =
			dynamic_cast<osgEarth::Util::EarthManipulator*>(this->mpRefGlobeWidgetEx->getCameraManipulator());
		if (earthManip)
		{
			double dltXPan = 0, dltYPan = 0,constDlt = 0.05;
			if (this->contains(this->mCameraMoveUpControl, ea.getX(), ea.getY()))
			{
				dltYPan = constDlt;
			}
			else if (this->contains(this->mCameraMoveDownControl, ea.getX(), ea.getY()))
			{
				dltYPan = -constDlt;
			}
			else if (this->contains(this->mCameraMoveLeftControl, ea.getX(), ea.getY()))
			{
				dltXPan = -constDlt;
			}
			else if (this->contains(this->mCameraMoveRightControl, ea.getX(), ea.getY()))
			{
				dltXPan = constDlt;
			}
			earthManip->pan(dltXPan, dltYPan);
		}
		
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == mMouseButton)
	{
		if (this->mCanSelectLevel)
		{
			this->mCanSelectLevel = false;
			this->mLevelSelectControl->setPosition(this->mLevelButtonX, this->mLevelButtonY);
			this->mLevelButtonX = -1;
			this->mLevelButtonY = -1;
			ea.setHandled(true);
		}

		if (this->getEagleVisible())
		{
			bool isInsideEagle = this->contains(this->mEagleImageControl, ea.getX(), ea.getY());
			if (isInsideEagle)
			{
				float halfH = this->mEaglePositionControl->height().get() / 2;
				float halfW = this->mEaglePositionControl->width().get() / 2;
				int pX = ea.getX() - halfW;
				int pY = this->mpRefGlobeWidgetEx->height() - ea.getY() - halfH;
				this->mEaglePositionControl->setPosition(pX, pY);

				float rX = pX - this->mEagleImageControl->x().get();
				float rY = pY - this->mEagleImageControl->y().get();

				double lon = -180 + (360 * rX) / this->mEagleImageControl->width().get();
				double lat = 90 - (180 * rY) / this->mEagleImageControl->height().get();
				this->mpRefGlobeWidgetEx->flyTo(osgEarth::Viewpoint("", lon, lat, 500000, 0, -90, 1.5e7), 1);
			}
		}
		
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
	{
		this->mCameraMoveUpControl->setVisible(this->contains(this->mCameraMoveUpControl, ea.getX(), ea.getY()));
		this->mCameraMoveDownControl->setVisible(this->contains(this->mCameraMoveDownControl, ea.getX(), ea.getY()));
		this->mCameraMoveLeftControl->setVisible(this->contains(this->mCameraMoveLeftControl, ea.getX(), ea.getY()));
		this->mCameraMoveRightControl->setVisible(this->contains(this->mCameraMoveRightControl, ea.getX(), ea.getY()));
		this->mEyeViewControl->setVisible(this->contains(this->mEyeViewControl, ea.getX(), ea.getY()));
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::DRAG)
	{
		if (this->mCanSelectLevel)
		{
			int mouseDownX = ea.getX();
			int mouseDownY = ea.getY();

			if (this->contains(this->mLevelControl,80, mouseDownX, mouseDownY))
			{
				int dy = this->mMouseDownY - mouseDownY;
				this->mLevelSelectControl->setPosition((int)(this->mLevelSelectControl->x().get()), (int)(this->mLevelSelectControl->y().get() + dy));
				float scale = (float)(dy) / 100;
				osg::ref_ptr<osgEarth::Util::EarthManipulator> pCameraOperator =
					dynamic_cast<osgEarth::Util::EarthManipulator*>(dynamic_cast<osgViewer::Viewer*>(this->mpRefGlobeWidgetEx->getViewer())->getCameraManipulator());
				pCameraOperator->zoom(0, scale);

				this->mMouseDownX = mouseDownX;
				this->mMouseDownY = mouseDownY;
			}		
			r = true; 
			ea.setHandled(true);
		}
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK)
	{
		int mouseDownX = ea.getX();
		int mouseDownY = ea.getY();

		if (this->contains(this->mEyeViewControl, mouseDownX, mouseDownY))
		{
			osg::ref_ptr<osgEarth::Util::EarthManipulator> pCameraOperator =
				dynamic_cast<osgEarth::Util::EarthManipulator*>(dynamic_cast<osgViewer::Viewer*>(this->mpRefGlobeWidgetEx->getViewer())->getCameraManipulator());
			osgEarth::Viewpoint vp("", 107.85, 32.35, 0.0, -2.50, -90.0, 1.5e7);
			pCameraOperator->setViewpoint(vp, 1);
			r = true;
			ea.setHandled(true);
		}
		
	}
	this->updateCompass();
	return r;
}

void CGlobeNavigation::setNavigationPosition(const int& x ,const int& y)
{
	this->mX = x;
	this->mY = y;
	if (this->mpRefGlobeWidgetEx)
	{
		this->resize(this->mpRefGlobeWidgetEx->width(), this->mpRefGlobeWidgetEx->height());
	}
	
}

void CGlobeNavigation::getNavigationPosition(int& x ,int& y)
{
	x = this->mX;
	y = this->mY;
}

void CGlobeNavigation::setGlobeScalePosition(const int& x ,const int& y)
{
	this->mGlobeScalePositionX = x;
	this->mGlobeScalePositionY = y;
	if (this->mpRefGlobeWidgetEx)
	{
		this->resize(this->mpRefGlobeWidgetEx->width(), this->mpRefGlobeWidgetEx->height());
	}
}

void CGlobeNavigation::getGlobeScalePosition(int& x ,int& y)
{
	x = this->mGlobeScalePositionX;
	y = this->mGlobeScalePositionY;
}

bool CGlobeNavigation::getGlobeScaleVisible()
{
	return this->mGlobeScaleVisible;
}

void CGlobeNavigation::setGlobeScaleVisible(const bool& visible)
{
	this->mGlobeScaleVisible = visible;
	if (this->mGlobeScaleControl)
	{
		this->mGlobeScaleControl->setVisible(visible);
		this->mScaleDisText->setNodeMask(visible);
		this->mGlobeScaleText->setNodeMask(visible);
	}
}

int CGlobeNavigation::getStateTextSize()
{
	if (this->mStateLabelText)
	{
		return this->mStateLabelText->getCharacterHeight();
	}
	return 0;
}


osgText::Text* CGlobeNavigation::getStateLabelText()
{
	return this->mStateLabelText;
}

osgText::Text* CGlobeNavigation::getGlobeScaleText()
{
	return this->mGlobeScaleText;
}

osgText::Text* CGlobeNavigation::getScaleDisText()
{
	return this->mScaleDisText;
}

void CGlobeNavigation::setStateTextSize(const int& size)
{
	if (this->mStateLabelText)
	{
		float textSize = size;
		this->mStateLabelText->setCharacterSize(textSize);
		this->mStateLabelText->setFontResolution(textSize,textSize);

		float stateControlHeight = textSize + 10;;
		this->mStateInfoControl->setPosition(0, this->mpRefGlobeWidgetEx->height() - stateControlHeight);
		this->mStateInfoControl->setHeight(stateControlHeight);
		if (this->mpRefGlobeWidgetEx)
		{
			this->mpRefGlobeWidgetEx->getViewer()->frame();
		}
	}
}

double CGlobeNavigation::getCurrentAltitude()
{
	return mMouseCoordsCallback->getCurrentAltitude();
}

//void CGlobeNavigation::setEagleEyeVisible(bool bVisible)
//{
//	osg::Group* root = mpRefGlobeWidgetEx->getRootRef();
//	bool bInScene = 
//		root->getChildIndex(this->mRadarCamera) != root->getNumChildren();
//	//显隐和是否在场景中一致，直接返回
//	if (bVisible == bInScene)
//		return;
//	if (bVisible)
//		root->addChild(mRadarCamera);
//	else
//		root->removeChild(mRadarCamera);
//}

