/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#include "localengine/ImageViewer.h"

/**
 * ImageViewer implementation
 */

namespace Engine
{
    CImageViewerManipulator::CImageViewerManipulator()
    {
        this->setAllowThrow(false);
        this->setVerticalAxisFixed(true);
    }

    CImageViewerManipulator::~CImageViewerManipulator()
    {
        
    }
    
    void CImageViewerManipulator::rotateTrackball(const float px0, const float py0,
        const float px1, const float py1, const float scale)
    {
        //osgGA::TrackballManipulator::rotateTrackball(px0,py0,px1,py1,scale);
    }

    void CImageViewerManipulator::rotateWithFixedVertical(const float dx, const float dy)
    {
        //osgGA::TrackballManipulator::rotateWithFixedVertical(dx, dy);
    }

    void CImageViewerManipulator::rotateWithFixedVertical(const float dx, const float dy, const osg::Vec3f& up)
    {
        //osgGA::TrackballManipulator::rotateWithFixedVertical(dx, dy, up);
    }

    CImageViewer::CImageViewer(QWidget * parent, const char * name, const QGLWidget * shareWidget, Qt::WindowFlags f)
        : I3DViewer(parent, name, shareWidget, f)
    {

    }

    CImageViewer::~CImageViewer()
    {

    }

    void CImageViewer::initial()
    {    
        I3DViewer::initial();
        {
            this->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(1500, 0, 0), 
                osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 0.0, 1.0));
            this->getCamera()->setClearColor(
                osg::Vec4(0.0, 90.0 / 255.0, 133.0 / 255.0, 1.0));

            CImageViewerManipulator* pManipulator = new CImageViewerManipulator();
            this->setCameraManipulator(pManipulator);
            this->getCameraManipulator()->setHomePosition(
                osg::Vec3d(700, 0, 0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 0.0, 1.0));
            this->getCameraManipulator()->home(1.0);
            osg::CullStack::CullingMode cullingMode = this->getCamera()->getCullingMode();
            cullingMode &= ~(osg::CullStack::SMALL_FEATURE_CULLING);
            this->getCamera()->setCullingMode(cullingMode);
           /* osg::Node* pAxis = this->createAxis();
            this->mpAxisNode =
                dynamic_cast<osg::AutoTransform*>(pAxis);
            this->getRoot()->addChild(pAxis);*/
        }
    }
}