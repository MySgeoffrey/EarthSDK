/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#include "localengine/SceneViewer.h"
#include "osg/MatrixTransform"
#include <osg/LineWidth>
#include <osgViewer/CompositeViewer>


/**
 * CSceneViewer implementation
 */

#define _CHANGEOPERATOR_ 1

namespace Engine
{    
    osg::ref_ptr<osg::Geode> createReferenceQuad()
    {
        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
        osg::ref_ptr<osg::Vec3Array> pts = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

        double length = 1000000;
        double z = -200;

        pts->push_back(osg::Vec3d(length, length, z));
        pts->push_back(osg::Vec3d(length, -length, z));
        pts->push_back(osg::Vec3d(-length, -length, z));
        pts->push_back(osg::Vec3d(-length, length, z));

        colors->push_back(osg::Vec4(0.0, 90.0 / 255.0, 133.0 / 255.0, 0.1));

        geometry->setVertexArray(pts.get());
        geometry->setColorArray(colors.get());
        geometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
        geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS, 0, pts->size()));
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        //开启融合操作
        geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        //设置渲染模式
        //geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        geode->getOrCreateStateSet()->setRenderBinDetails(-1, "RenderBin");
        geode->addDrawable(geometry);
        return geode.release();
    }

    CSceneViewerManipulator::CSceneViewerManipulator()
    {
        this->mMouseDownPos = osg::Vec3d(0, 0, 0);
        this->mCurMousePos = osg::Vec3d(0, 0, 0);
        this->mCurMouseIntersectPos = osg::Vec3d(0, 0, 0);
        this->mMouseDownIntersectPos = osg::Vec3d(0, 0, 0);
        this->mMoveDirection = osg::Vec3d(0, 0, 0);
        this->mMouseDragPos = osg::Vec3d(0, 0, 0);
        this->mDistanceToTarget = 0;
        this->mMouseType = -1;
        this->SceneViewer = NULL;
    }

    CSceneViewerManipulator::~CSceneViewerManipulator()
    {

    }

    void CSceneViewerManipulator::setByMatrix(const osg::Matrixd& matrix)
    {
        osgGA::TrackballManipulator::setByMatrix(matrix);
    }

    // 射线与三角形求交V0，V1，V2三角形三顶点 I为交点
    bool rayIntersect(
        osg::Vec3d v0,
        osg::Vec3d v1,
        osg::Vec3d v2,
        const osg::Vec3d& origin, const osg::Vec3d& rayDir, osg::Vec3d& interPoint)
    {
        // 射线与三角形平行? 
        osg::Vec3d u = v1 - v0;    // edge1
        osg::Vec3d v = v2 - v0;    // edge2
        osg::Vec3d norm = u ^ v;  // normal

        if (norm == osg::Vec3d(0.0f, 0.0f, 0.0f))  // triangle is degenerate 
            return false;

        // 计算射线与平面法向夹角
        float b = norm * rayDir;
        if (fabs(b) < 0.000005)      // ray is parallel to triangle plane   
            return false;

        // 计算v0到射线起始点的向量
        osg::Vec3d w0 = origin - v0;
        float a = -(norm * w0);

        // get intersect point of ray with triangle plane   
        float r = a / b;
        if (r < 0.0f)                 // ray goes away from triangle   
            return false;                 // => no intersect   
        // for a segment, also test if (r > 1.0) => no intersect   

        // 计算射线与平面的交点
        interPoint = origin + rayDir * r;

        // 计算交点是否在三角形内部?   
        float uu = u * u;
        float uv = u * v;
        float vv = v * v;
        osg::Vec3d w = interPoint - v0;
        float wu = w * u;
        float wv = w * v;
        float D = uv * uv - uu * vv;

        // get and test parametric coords   
        float s = (uv * wv - vv * wu) / D;
        if (s < 0.0f || s > 1.0f)       // I is outside T   
            return false;

        float t = (uv * wu - uu * wv) / D;
        if (t < 0.0f || (s + t) > 1.0f) // I is outside T   
            return false;

        return true;   // 交点在三角形内部 
    }

    osg::Vec3d screen2World(osg::Vec3 screenPoint, osgViewer::Viewer* pViewer)//将屏幕坐标转换到世界坐标
    {
        osg::Vec3d vec3;
        osg::ref_ptr<osg::Camera> camera = pViewer->getCamera();
        //osg::Vec3d vScreen(x,y, 0);
        osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
        osg::Matrix invertVPW;
        invertVPW.invert(mVPW);
        vec3 = screenPoint * invertVPW;
        return vec3;
    }

    osg::Vec3d CSceneViewerManipulator::getIntersectedPoint(
        float x, float y, osgGA::GUIActionAdapter& us,
        int mouseType)
    {
        osg::Vec3d r(0, 0, 0);
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&us);
        if (NULL == pViewer)
            return r;
        float ex = x;
        float ey = y;
        if (mouseType != 1)
        {
            float xMin = ex - 5.0;
            float yMin = ey - 5.0;
            float xMax = ex + 5.0;
            float yMax = ey + 5.0;
            osg::ref_ptr<osgUtil::PolytopeIntersector> picker = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, xMin, yMin, xMax, yMax);
            osgUtil::IntersectionVisitor iv(picker.get());
            pViewer->getCamera()->accept(iv);
            if (picker->containsIntersections())
            {
#if 1
                osg::RefMatrixd& m = *picker->getFirstIntersection().matrix;
                r = picker->getFirstIntersection().intersectionPoints[0] * m;
#else
                osgUtil::PolytopeIntersector::Intersections::iterator
                    hitr = picker->getIntersections().begin();
                const osg::NodePath& np = hitr->nodePath;
                for (int i = np.size() - 1; i >= 0; --i)
                {
                    osg::ref_ptr<osg::Node> node = dynamic_cast<osg::Node *>(np[i]);
                    if (NULL != node)
                    {
                        DataRender::CRenderNode* pRenerNode =
                            dynamic_cast<DataRender::CRenderNode*>(node.get());
                        if (pRenerNode && pRenerNode->Owner)
                        {
                            r = picker->getFirstIntersection().intersectionPoints[0];
                        }
                    }
                }
#endif
            }
            else
            {
                osg::Vec3 start(ex, ey, 0);
                osg::Vec3 end(ex, ey, 1);
                start = screen2World(start, pViewer);
                end = screen2World(end, pViewer);
                osg::ref_ptr<osg::Vec3Array> pts = new osg::Vec3Array;
                osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;


                double length = 1000000;
    //            osg::Vec3d eye = osg::Vec3d(0, 0, 0);
    //            osg::Vec3d center = osg::Vec3d(0, 0, 0);
    //            osg::Vec3d up = osg::Vec3d(0, 0, 0);
    //            //this->getTransformation(eye,center,up);
				//this->getTransformation(eye, center, up);
				//double z = (center.z() + eye.z()) / 2;
				double z = -1;
                

                pts->push_back(osg::Vec3d(length, length, z));
                pts->push_back(osg::Vec3d(length, -length, z));
                pts->push_back(osg::Vec3d(-length, -length, z));
                pts->push_back(osg::Vec3d(-length, length, z));
                osg::Vec3d ray(end - start);
                ray.normalize();
                if (!rayIntersect(pts->at(0), pts->at(1), pts->at(2), start, ray, r))
                {
                    rayIntersect(pts->at(0), pts->at(2), pts->at(3), start, ray, r);
                    if (r != osg::Vec3d(0, 0, 0))
                    {
                        int x = 0;
                    }
                }
            }
        }
        else
        {
            osg::Vec3 start(ex, ey, 0);
            osg::Vec3 end(ex, ey, 1);
            start = screen2World(start, pViewer);
            end = screen2World(end, pViewer);
            osg::ref_ptr<osg::Vec3Array> pts = new osg::Vec3Array;
            osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

            double length = 100000000;
            double z = -1;// this->getCenter().z();
			//osg::Vec3d eye = osg::Vec3d(0, 0, 0);
			//osg::Vec3d center = osg::Vec3d(0, 0, 0);
			//osg::Vec3d up = osg::Vec3d(0, 0, 0);
			////this->getTransformation(eye,center,up);
			//this->getTransformation(eye, center, up);
			//double z = (center.z() + eye.z()) / 2;
            pts->push_back(osg::Vec3d(length, length, z));
            pts->push_back(osg::Vec3d(length, -length, z));
            pts->push_back(osg::Vec3d(-length, -length, z));
            pts->push_back(osg::Vec3d(-length, length, z));
            osg::Vec3d ray(end - start);
            ray.normalize();
            if (!rayIntersect(pts->at(0), pts->at(1), pts->at(2), start, ray, r))
            {
                rayIntersect(pts->at(0), pts->at(2), pts->at(3), start, ray, r);
                if (r != osg::Vec3d(0, 0, 0))
                {
                    int x = 0;
                }
            }
        }
        return r;
    }

    osg::Vec3d CSceneViewerManipulator::getIntersectedPoint(
        const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us,
        int mouseType)
    {
        osg::Vec3d r(0, 0, 0);
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&us);
        if (NULL == pViewer)
            return r;
        float ex = ea.getX();
        float ey = ea.getY();
        if (std::abs(ex) < 0.00001 || std::abs(ey) < 0.00001)
        {
            ex = ea.getScrollingDeltaX();
            ey = ea.getScrollingDeltaY();
        }
        return this->getIntersectedPoint(ex, ey, us, mouseType);
    }

    bool CSceneViewerManipulator::handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        bool r = false;
        r = osgGA::TrackballManipulator::handleMouseMove(ea, us);
        return r;
    }

    bool CSceneViewerManipulator::handleMouseDrag(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        bool r = false;
        r = osgGA::TrackballManipulator::handleMouseDrag(ea, us);
        return r;
    }

    bool CSceneViewerManipulator::handleMousePush(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        bool r = false;
        r = osgGA::TrackballManipulator::handleMousePush(ea, us);
        return r;
    }

    bool CSceneViewerManipulator::handleMouseRelease(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        bool r = false;
        r = osgGA::TrackballManipulator::handleMouseRelease(ea, us);
        return r;
    }
    bool CSceneViewerManipulator::handleMouseWheel(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        bool r = false;
        r = osgGA::TrackballManipulator::handleMouseWheel(ea, us);
        return r;
    }

    bool CSceneViewerManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        if (ea.getHandled()) 
            return false;
        bool r = false;
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&us);
        if (NULL == pViewer)
            return r;
#if _CHANGEOPERATOR_ == 0
        r = osgGA::TrackballManipulator::handle(ea, us);
        return r;
#else
        
        switch (ea.getEventType())
        {
        case(osgGA::GUIEventAdapter::PUSH) :
        {
            mMouseDownPos = osg::Vec3d(ea.getX(), ea.getY(), 0);
            mCurMousePos = osg::Vec3d(ea.getX(), ea.getY(), 0);
            this->mMouseDragPos = mMouseDownPos;
            bool needIntersect = false;
            this->SceneViewer->updateMouseIcon(ea.getX(), ea.getY(), false);
            if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
            {
                this->mMouseType = 1;
                needIntersect = true;
            }
            else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
            {
                this->mMouseType = 3;
                needIntersect = true;
            }
            else if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
            {
                this->mMouseType = 2;
                needIntersect = true;
            }

            if (needIntersect)
            {
                this->mCurMouseIntersectPos = this->getIntersectedPoint(ea, us, 2);//this->mMouseType
				if (this->mCurMouseIntersectPos != osg::Vec3d(0, 0, 0))
                {
                    r = true;
                    if (this->mMouseType == 3)
                    {
                        this->SceneViewer->updateMouseIcon(ea.getX(), ea.getY(), true);
                    }
                }
            }
            break;
        }
        case(osgGA::GUIEventAdapter::DRAG) :
        {
            osg::Vec3d eye = osg::Vec3d(0, 0, 0);
            osg::Vec3d center = osg::Vec3d(0, 0, 0);
            osg::Vec3d up = osg::Vec3d(0, 0, 0);
            osgGA::TrackballManipulator* pManipulator =
                dynamic_cast<osgGA::TrackballManipulator*>(pViewer->getCameraManipulator());
            pManipulator->getTransformation(eye, center, up);
            if (this->mCurMouseIntersectPos == osg::Vec3d(0, 0, 0))
                break;
            if (this->mMouseType == 1)
            {
#if 1
                this->mDistanceToTarget = (this->mCurMouseIntersectPos - eye).length();
                double deltaX = this->mCurMousePos.x() - ea.getX();
                double deltaY = this->mCurMousePos.y() - ea.getY();
                double factorX = 0.0022;
				double factorY = 0.0022;

                osg::Vec3d lookDir = center - eye;
                lookDir.normalize();
                osg::Vec3d crossAxis = lookDir ^ osg::Vec3d(0, 0, 1);
                crossAxis.normalize();

                osg::Vec3d forwardDir = osg::Vec3d(0, 0, 1);
                forwardDir = forwardDir ^ crossAxis;
                forwardDir.normalize();

                osg::Vec3d moveDir = crossAxis * deltaX * this->mDistanceToTarget * factorX + forwardDir * deltaY * this->mDistanceToTarget * factorY;
                eye += moveDir;
                center += moveDir;

                this->setTransformation(eye, center, up);
                this->mCurMousePos = osg::Vec3d(ea.getX(), ea.getY(), 0);
#else
                double delta = (this->mCurMousePos.x() - ea.getX()) * (this->mCurMousePos.x() - ea.getX())
                    + (this->mCurMousePos.y() - ea.getY()) * (this->mCurMousePos.y() - ea.getY());
                if (delta > 4 && this->mCurMouseIntersectPos != osg::Vec3d(0, 0, 0))
                {
					//osg::Vec3d curMouseIntersectPos = getIntersectedPoint(ea, us,2); //this->getIntersectedPoint(ea, us);
					osg::Vec3d curMouseIntersectPos = screen2World(osg::Vec3(ea.getX(), ea.getY(), 0), this->SceneViewer);
					if (curMouseIntersectPos != osg::Vec3d(0, 0, 0))
                    {
                        osg::Vec3d moveDir = curMouseIntersectPos - this->mCurMouseIntersectPos;
                        moveDir.z() = 0;
                        eye -= moveDir;
                        center -= moveDir;

                        this->setTransformation(eye, center, up);
                        this->mCurMousePos = osg::Vec3d(ea.getX(), ea.getY(), 0);
                        r = true;
                    }
                }
#endif
                if (this->SceneViewer)
                {
                    this->SceneViewer->updateStateText();
                }
            }
            else if (this->mMouseType == 3)
            {
                osg::Quat qX;
                osg::Quat qY;
                double factor = 0.3;
                double flagX = ea.getX() - this->mMouseDragPos.x();
                double flagY = ea.getY() - this->mMouseDragPos.y();

                osg::Vec3d xAxis = osg::Vec3d(0, 0, 1);
                osg::Vec3d yAxis = osg::Vec3d(0, 0, 1);
                osg::Vec3d lookDir = center - eye;
                lookDir.normalize();
                yAxis = lookDir ^ osg::Vec3d(0, 0, 1);
                yAxis.normalize();
                double angle = factor * 3.1415926 / 180.0;
                qX.makeRotate(-angle * flagX, xAxis);
                qY.makeRotate(angle * flagY, yAxis);

                osg::Vec3d referenceCenter = this->mCurMouseIntersectPos;
                eye = qX * qY * (eye - referenceCenter) + referenceCenter;
                center = qX * qY * (center - referenceCenter) + referenceCenter;
                up = qX * qY * up;
                double a = std::acos(up * osg::Vec3d(0, 0, 1));
                a = std::abs(a * 180 / 3.1415926);

                if (a < 85)
                {
                    this->setTransformation(eye, center, up);
                    if (this->SceneViewer)
                    {
                        this->SceneViewer->updateStateText();
                    }
                }
            }
            
            this->mMouseDragPos = osg::Vec3d(ea.getX(), ea.getY(), 0);
            r = true;
            break;
        }
        case(osgGA::GUIEventAdapter::MOVE) :
        {
            osg::Vec3d pos = osg::Vec3d(ea.getX(), ea.getY(), 0);
            double moveDelta = (mCurMousePos.x() - pos.x()) * (mCurMousePos.x() - pos.x())
                + (mCurMousePos.y() - pos.y()) * (mCurMousePos.y() - pos.y());
            if (moveDelta > 4)
            {
                this->mCurMouseIntersectPos = osg::Vec3d(0, 0, 0);
                this->mCurMousePos = pos;
            }
            this->SceneViewer->updateMouseIcon(ea.getX(), ea.getY(), false);
            break;
        }
        case(osgGA::GUIEventAdapter::SCROLL) :
        {
            double flag = 1.0;
            osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
            if (sm == osgGA::GUIEventAdapter::SCROLL_DOWN)
            {
                flag = 1.0;
            }
            else if (sm == osgGA::GUIEventAdapter::SCROLL_UP)
            {
                flag = -1.0;
            }
            else if (sm == osgGA::GUIEventAdapter::SCROLL_2D)
            {
                if (this->mCurMouseIntersectPos == osg::Vec3d(0,0,0))
                {
                    this->mCurMouseIntersectPos = this->getIntersectedPoint(mCurMousePos.x(), mCurMousePos.y(),us, 3);
                }
                flag = -1000;
            }
            if (flag > -500)
            {
                osg::Vec3d eye = osg::Vec3d(0, 0, 0);
                osg::Vec3d center = osg::Vec3d(0, 0, 0);
                osg::Vec3d up = osg::Vec3d(0, 0, 0);
                pViewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);

                if (this->mCurMouseIntersectPos != osg::Vec3d(0, 0, 0))
                {
                    osg::Vec3d moveDir = this->mCurMouseIntersectPos - eye;
                    this->mDistanceToTarget = moveDir.normalize();
                    this->mMoveDirection = moveDir;
                    flag *= 0.15;

                    double moveDistance = this->mDistanceToTarget ;
                    eye += this->mMoveDirection * moveDistance * flag;
                    center += this->mMoveDirection * moveDistance * flag;
                    this->setTransformation(eye, center, up);
                    this->SceneViewer->updateMouseIcon(ea.getX(), ea.getY(), true);
                }
            }
            if (this->SceneViewer)
            {
                this->SceneViewer->updateStateText();
            }
            break;
        }
        case(osgGA::GUIEventAdapter::RELEASE) :
        {
            this->mCurMouseIntersectPos = osg::Vec3d(0, 0, 0);
            this->mMouseDownPos = osg::Vec3d(0, 0, 0);
            this->mCurMousePos = osg::Vec3d(0, 0, 0);
            this->mCurMouseIntersectPos = osg::Vec3d(0, 0, 0);
            this->mMouseDownIntersectPos = osg::Vec3d(0, 0, 0);
            this->mMoveDirection = osg::Vec3d(0, 0, 0);
            this->mMouseDragPos = osg::Vec3d(0, 0, 0);
            this->mDistanceToTarget = 0;
            this->mMouseType = -1;
            this->mIsMouseMove = false;
            this->SceneViewer->updateMouseIcon(ea.getX(), ea.getY(), false);
            break;
        }
        }
        if (!r)
        {
            r = osgGA::TrackballManipulator::handle(ea, us);
        }
        
#endif
        return r;
    }

    
    CSceneViewer::CSceneViewer(QWidget * parent, const char * name, const QGLWidget * shareWidget, Qt::WindowFlags f)
        : I3DViewer(parent, name, shareWidget, f), mpStateNode(NULL), mpStateText(NULL)
    {
        
    }

    CSceneViewer::~CSceneViewer()
    {
        if (this->mpStateNode)
        {
            this->getRoot()->removeChild(this->mpStateNode);
            this->mpStateNode = NULL;
        }
    }

    void CSceneViewer::resizeGL(int width, int height)
    {
        I3DViewer::resizeGL(width,height);
        if (this->mpStateNode)
        {
            this->mpStateNode->asCamera()->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
        }
        if (this->mpHudCamera)
        {
            this->mpHudCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
        }
    }

    void CSceneViewer::updateMouseIcon(const double& x, const double& y, const bool& visible)
    {
        if (this->mpHudCamera)
        {
            osg::Node* pImageNode = this->mpHudCamera->getChild(0);
            if (pImageNode)
            {
                osg::MatrixTransform* pTransform =
                    dynamic_cast<osg::MatrixTransform*>(pImageNode);
                if (pTransform)
                {
                    pTransform->setMatrix(osg::Matrix::translate(x,y,0));
                }
            }
            this->mpHudCamera->setNodeMask(visible);
        }

    }

    void CSceneViewer::updateStateText()
    {
        if (this->mpStateNode)
        {
            CSceneViewerManipulator* pCameraManipulator
                = dynamic_cast<CSceneViewerManipulator*>(this->getCameraManipulator());
            if (pCameraManipulator)
            {
                osgText::Text* pText =
                    dynamic_cast<osgText::Text*>(this->mpStateNode->asGroup()->getChild(0)->asGeode()->getDrawable(0));
                if (pText)
                {
                    osg::Vec3d eye = osg::Vec3d(0, 0, 0);
                    osg::Vec3d center = osg::Vec3d(0, 0, 0);
                    osg::Vec3d up = osg::Vec3d(0, 0, 0);
                    pCameraManipulator->getTransformation(eye,center,up);
                    double x = eye.x();//(int)
                    double y = eye.y();//(int)
                    double z = eye.z();//(int)
                    std::string stateInfo = "Camera ";
                    char info[256];
                    std::sprintf(info, "%s X:%.0f  Y:%.0f  Z:%.0f\n", stateInfo.c_str(), x,y,z);
                    pText->setText(info);
                }
            }
            
        }
        
    }

    void CSceneViewer::initial()
    {
        I3DViewer::initial();
        {
            CSceneViewerManipulator* pManipulator = new CSceneViewerManipulator();
            pManipulator->SceneViewer = this;
            this->setCameraManipulator(pManipulator);
            this->getCamera()->setClearColor(osg::Vec4(0.0, 90.0 / 255.0, 133.0 / 255.0, 1.0));
			//this->getCamera()->setClearColor(osg::Vec4(0.0, 140.0 / 255.0, 150.0 / 255.0, 1.0));
            osg::CullStack::CullingMode cullingMode = this->getCamera()->getCullingMode();
            cullingMode &= ~(osg::CullStack::SMALL_FEATURE_CULLING);
            this->getCamera()->setCullingMode(cullingMode);
            pManipulator->setAllowThrow(false);
            pManipulator->setVerticalAxisFixed(true);
            pManipulator->setHomePosition(osg::Vec3d(-250, 250, 250), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));
            pManipulator->setTransformation(osg::Vec3d(-250, 250, 250), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));
            pManipulator->home(1.0);
            this->setCameraManipulator(pManipulator);
            double dis = pManipulator->getDistance();
            osg::Node* pAxis = this->createAxis();
            this->mpAxisNode =
                dynamic_cast<osg::AutoTransform*>(pAxis);
            this->getRoot()->addChild(pAxis);

            std::vector<osgViewer::GraphicsWindow*> windows;
            this->getWindows(windows);
            osgViewer::GraphicsWindow* pGraphicsWindow = windows.at(0);
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
            pGraphicsWindow->getWindowRectangle(x, y, width, height);
            width = 1280;
            height = 768;
            osg::Vec4 color = osg::Vec4(1, 1, 1, 1);
            std::string font = "resource/fonts/simhei.ttf";
            this->mpStateNode = this->createHUDTextEx(40, 20, width, height, 20.0, "", color, font);
            this->getRoot()->addChild(this->mpStateNode);
            this->updateStateText();

            this->mpHudCamera = this->createHUDImage(0, 0, 1280, 768)->asCamera();
            if (this->mpHudCamera)
            {
                this->getRoot()->addChild(this->mpHudCamera);
                this->mpHudCamera->setNodeMask(0);
            }
            
            /*osg::ref_ptr<osg::Geode> pNode = createReferenceQuad();
            this->getRoot()->addChild(pNode);*/
            /*osg::Node* pGrid = this->createGrid(40, 40, 50);
            this->getRoot()->addChild(pGrid);*/
        }
    }
}
