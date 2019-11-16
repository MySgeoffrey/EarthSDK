#include "localengine/I3DViewer.h"
#include "osgGA/TrackballManipulator"
#include "osg/MatrixTransform"
#include <osg/LineWidth>
#include <osg/Camera>

namespace Engine
{
    

    IAdapterWidget::IAdapterWidget(QWidget *parent, const char* name, 
        const QGLWidget * shareWidget, Qt::WindowFlags f) 
        :QGLWidget(parent, shareWidget, f)
    {
        _gw = new osgViewer::GraphicsWindowEmbedded(0, 0, width(), height());
        setFocusPolicy(Qt::ClickFocus);
        setMouseTracking(TRUE);
    }

    void IAdapterWidget::resizeGL(int width, int height)
    {
        _gw->getEventQueue()->windowResize(0, 0, width, height);
        _gw->resized(0, 0, width, height);

    }
    void IAdapterWidget::keyPressEvent(QKeyEvent* event)
    {
        _gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
    }

    void IAdapterWidget::keyReleaseEvent(QKeyEvent* event)
    {
        int keyAscii = (osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data());
        _gw->getEventQueue()->keyRelease(keyAscii);
        //if (keyAscii == 27)
        //    quit();
    }

    void IAdapterWidget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        int button = 0;
        switch (event->button())
        {
        case(Qt::LeftButton) :
            button = 1;
            break;
        case (Qt::MidButton) :
            button = 2;
            break;
        case (Qt::RightButton) :
            button = 3;
            break;
        case (Qt::NoButton) :
            button = 0;
            break;
        default:
            button = 0;
            break;
        }

        _gw->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
    }

    void IAdapterWidget::mousePressEvent(QMouseEvent* event)
    {
        int button = 0;
        switch (event->button())
        {
        case(Qt::LeftButton) :
            button = 1;
            break;
        case (Qt::MidButton) :
            button = 2;
            break;
        case (Qt::RightButton) :
            button = 3;
            break;
        case (Qt::NoButton) :
            button = 0;
            break;
        default:
            button = 0;
            break;
        }

        _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);

    }

    void IAdapterWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        int button = 0;
        switch (event->button())
        {
        case(Qt::LeftButton) :
            button = 1;
            break;
        case(Qt::MidButton) :
            button = 2;
            break;
        case(Qt::RightButton) :
            button = 3;
            break;
        case(Qt::NoButton) :
            button = 0;
            break;
        default:
            button = 0;
            break;
        }
        _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
    }

    void IAdapterWidget::wheelEvent(QWheelEvent * event)
    {
        float x = event->x();
        float y = event->y();
        _gw->getEventQueue()->mouseScroll2D(x, y);
        if (event->delta() > 0)
        {
            _gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
        }
        else
        {
            _gw->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
        }
    }

    void  IAdapterWidget::mouseMoveEvent(QMouseEvent* event)
    {
        _gw->getEventQueue()->mouseMotion(event->x(), event->y());

    }

    I3DViewer::I3DViewer(QWidget * parent, const char * name, const QGLWidget * shareWidget, Qt::WindowFlags f)
        :IAdapterWidget(parent, name, shareWidget, f), mpAxisNode(NULL), mFrameState(true)
    {
        this->setMinimumSize(QSize(10, 10));
        osg::ref_ptr< osg::DisplaySettings > displaySettings = new osg::DisplaySettings;
        displaySettings->setNumMultiSamples(16);
        this->setDisplaySettings(displaySettings.get());

        int w = width();
        int h = height();
        getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
        getCamera()->setProjectionMatrixAsPerspective(45.0f, static_cast<double>(width()) / static_cast<double>(height()), 1.0f, 10000.0f);
        getCamera()->setGraphicsContext(getGraphicsWindow());
        mpRoot = new osg::Group();

        this->setSceneData(this->mpRoot);

        setThreadingModel(osgViewer::Viewer::SingleThreaded);
        connect(&mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));//并且把它的timeout()连接到适当的槽。当这段时间过去了，它将会发射timeout()信号。
        mTimer.start(10);//使用start()来开始

    }

    I3DViewer::~I3DViewer()
    {
        if (this->mpAxisNode)
        {
            this->getRoot()->removeChild(this->mpAxisNode);
            this->mpAxisNode = NULL;
        }
    }

    osg::Camera* createHUDCamera(double left, double right, double bottom, double top)
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setClearMask(GL_DEPTH_BUFFER_BIT);
        camera->setRenderOrder(osg::Camera::POST_RENDER);
        camera->setAllowEventFocus(false);
        camera->setProjectionMatrix(osg::Matrix::ortho2D(left, right, bottom, top));
        camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        return camera.release();
    }

    std::string GBKToUTF8(const std::string &strGBK)
    {
        std::string strOutUTF8 = "";
        int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
        WCHAR * str1 = new WCHAR[n * 2];
        MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
        n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
        std::wstring wstr(str1);
        //strOutUTF8 = WStrToStr(wstr);
        char* str2 = new char[n];
        WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
        strOutUTF8 = str2;

        delete[] str1;
        delete[] str2;
        str1 = NULL;
        str2 = NULL;

        return strOutUTF8;
    }

    void setUTF8Text(osgText::Text* text, const std::string& content)
    {
        std::string utf8str = GBKToUTF8(content);
        text->setText(utf8str, osgText::String::ENCODING_UTF8);
    }

    osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size, const std::string &font)
    {
        osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile(font);
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setDataVariance(osg::Object::DYNAMIC);
        text->setFont(g_font.get());
        text->setCharacterSize(size);
        text->setAxisAlignment(osgText::TextBase::XY_PLANE);
        text->setPosition(pos);
        setUTF8Text(text.get(), content);
        return text.release();
    }

    osg::Node* I3DViewer::createHUDTextEx(const float &x, const float &y, const float &width,
        const float &height, const float &size, const std::string &content, const osg::Vec4 &color, const std::string &font)
    {
        osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera(0, width, 0, height);
        osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
        hudCamera->addChild(textGeode);
        osg::ref_ptr<osgText::Text>  text = createText(osg::Vec3(x, y, 0), content, size, font);
        text->setColor(color);

        /// 设置信息提示节点的属性，并关闭光照和深度测试
        osg::StateSet* stateset = textGeode->getOrCreateStateSet();
        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
        text->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
        text->setBackdropColor(osg::Vec4(0, 0, 0, 1));
        text->setBackdropType(osgText::Text::BackdropType::OUTLINE);
        /// 将文本添加到绘制列表中
        textGeode->addDrawable(text.get());
        return hudCamera.release();
    }

    osg::Node* I3DViewer::createHUDImage(const float &x, const float &y, const float &width,const float &height)
    {
        osg::ref_ptr<osg::Image> image = osgDB::readImageFile("resource/image/mouse.png");
        if (image == NULL)
            return NULL;
        osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera(0, width, 0, height);
        osg::ref_ptr<osg::Geode> imageGeode = new osg::Geode;

        osg::MatrixTransform* pImageNode = new osg::MatrixTransform();
        pImageNode->addChild(imageGeode);

        hudCamera->addChild(pImageNode);

        osg::ref_ptr<osg::Geometry> gm = new osg::Geometry();
        imageGeode->addDrawable(gm);
        //压入顶点
        double imageWidth = image->s() / 2;
        double imageHeight = image->t() / 2;
        osg::Vec2d pos(-imageWidth / 2, -imageHeight / 2);
        osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();
        vertex->push_back(osg::Vec3(0.0     + pos.x(),    0.0     + pos.y(),    0.0));
        vertex->push_back(osg::Vec3(imageWidth + pos.x(), 0.0 + pos.y(), 0.0));
        vertex->push_back(osg::Vec3(imageWidth + pos.x(), imageHeight + pos.y(), 0.0));
        vertex->push_back(osg::Vec3(0.0 + pos.x(), imageHeight + pos.y(), 0.0));

        gm->setVertexArray(vertex);

        //法线
        osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
        normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
        gm->setNormalArray(normal);

        //设置绑定方式
        gm->setNormalBinding(osg::Geometry::BIND_OVERALL);

        //设置纹理坐标
        osg::ref_ptr<osg::Vec2Array> coord = new osg::Vec2Array();
        coord->push_back(osg::Vec2(0.0, 0.0));
        coord->push_back(osg::Vec2(1.0, 0.0));
        coord->push_back(osg::Vec2(1.0, 1.0));
        coord->push_back(osg::Vec2(0.0, 1.0));
        gm->setTexCoordArray(0, coord);

        //设置关联
        gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

        //贴纹理
        osg::ref_ptr<osg::Texture2D> t2d = new osg::Texture2D();
        t2d->setImage(0, image);

        gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, t2d, osg::StateAttribute::ON);
        gm->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        gm->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
        gm->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        //设置渲染模式
        gm->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        return hudCamera.release();
    }

    osg::Node* I3DViewer::createHUDText()
    {
        ////文字
        //osgText::Text* text = new osgText::Text;
        ////text->setDataVariance(osg::Object::DYNAMIC);
        ////设置字体
        ////std::string caiyun("fonts /STCAIYUN.TTF");//此处设置的是汉字字体
        ////text->setFont(caiyun);
        ////设置文字显示的位置
        //osg::Vec3 position(150.0f, 100.0f, 0.0f);
        //text->setPosition(position);
        //text->setColor(osg::Vec4(1, 1, 0, 1));
        //text->setText(L"abc");//设置显示的文字
        ////几何体节点
        //osg::Geode* geode = new osg::Geode();
        //geode->addDrawable(text);//将文字Text作这drawable加入到Geode节点中
        ////设置状态
        //osg::StateSet* stateset = geode->getOrCreateStateSet();
        //stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);//关闭灯光
        //stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);//关闭深度测试
        ////打开GL_BLEND混合模式（以保证Alpha纹理正确）
        //stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
        ////相机
        //osg::Camera* camera = new osg::Camera;
        ////设置透视矩阵
        //camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 600));//正交投影   
        ////设置绝对参考坐标系，确保视图矩阵不会被上级节点的变换矩阵影响
        //camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        ////视图矩阵为默认的
        //camera->setViewMatrix(osg::Matrix::identity());
        ////设置背景为透明，否则的话可以设置ClearColor 
        //camera->setClearMask(GL_DEPTH_BUFFER_BIT);
        //camera->setAllowEventFocus(false);//不响应事件，始终得不到焦点
        ////设置渲染顺序，必须在最后渲染
        //camera->setRenderOrder(osg::Camera::POST_RENDER);
        //camera->addChild(geode);//将要显示的Geode节点加入到相机
        //return camera;
		return NULL;
    };

    void I3DViewer::paintGL()
    {
        if (this->mFrameState)
        {
            frame();
        }
    }

    void I3DViewer::initial()
    {

    }

    void I3DViewer::updateAxisPosition(const osg::Vec3d& in_pos)
    {
        if (NULL != mpAxisNode)
        {
            mpAxisNode->setPosition(in_pos);
        }
    }

    osg::ref_ptr<osg::Group> I3DViewer::getRoot()
    {
        return this->mpRoot;
    }

    void I3DViewer::setVisible(const bool& in_visible)
    {
        this->mFrameState = in_visible;
        IAdapterWidget::setVisible(in_visible);
    }

    osg::Node* I3DViewer::createAxis()
    {
        osg::ref_ptr<osg::Geode> geodeAxis = new osg::Geode;
        geodeAxis->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        geodeAxis->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.f));
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
        geodeAxis->addDrawable(geometry.get());

        osg::ref_ptr<osg::Vec3Array> pts = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

        double shortLength = 0.2;
        double longLength = 0.8;
        double factor = 80;

        //x轴
        pts->push_back(osg::Vec3(0, 0, 0));
        pts->push_back(osg::Vec3(1 * factor, 0, 0));
        pts->push_back(osg::Vec3(1 * factor, 0, 0));
        pts->push_back(osg::Vec3(longLength * factor, -shortLength * factor, 0));
        pts->push_back(osg::Vec3(1 * factor, 0, 0));
        pts->push_back(osg::Vec3(longLength * factor, shortLength * factor, 0));

        //Y轴
        pts->push_back(osg::Vec3(0, 0, 0));
        pts->push_back(osg::Vec3(0, 1 * factor, 0));
        pts->push_back(osg::Vec3(0, 1 * factor, 0));
        pts->push_back(osg::Vec3(-shortLength * factor, longLength * factor, 0));
        pts->push_back(osg::Vec3(0, 1 * factor, 0));
        pts->push_back(osg::Vec3(shortLength * factor, longLength * factor, 0));

        //Z轴
        pts->push_back(osg::Vec3(0, 0, 0));
        pts->push_back(osg::Vec3(0, 0, 1 * factor));
        pts->push_back(osg::Vec3(0, 0, 1 * factor));
        pts->push_back(osg::Vec3(-shortLength * factor, 0, longLength * factor));
        pts->push_back(osg::Vec3(0, 0, 1 * factor));
        pts->push_back(osg::Vec3(shortLength * factor, 0, longLength * factor));

        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));

        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));

        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        colors->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));

        geometry->setVertexArray(pts.get());
        geometry->setColorArray(colors.get());
        geometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
        geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, pts->size()));
        osg::ref_ptr<osg::AutoTransform> pAutoNode = new osg::AutoTransform();
        pAutoNode->addChild(geodeAxis);
        pAutoNode->setAutoRotateMode(osg::AutoTransform::NO_ROTATION);
        pAutoNode->setAutoScaleToScreen(true);
        pAutoNode->setPosition(osg::Vec3d(0, 0, 0));
        return pAutoNode.release();
    }
}