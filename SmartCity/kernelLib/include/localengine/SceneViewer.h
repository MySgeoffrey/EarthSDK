/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#ifndef _SCENEVIEWER_H
#define _SCENEVIEWER_H

#include "localengine/EngineConfig.h"
#include "localengine/I3DViewer.h"
#include "osgGA/TrackballManipulator"
#include "osgGA/GUIEventHandler"

namespace Engine
{
    class CSceneViewer;
    class VR_LOCAL_ENGINE_EXPORT CSceneViewerManipulator : public osgGA::TrackballManipulator
    {
    public:
        CSceneViewerManipulator();

        virtual ~CSceneViewerManipulator();

        virtual void setByMatrix(const osg::Matrixd& matrix);

        virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

        virtual bool handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        virtual bool handleMouseDrag(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        virtual bool handleMousePush(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        virtual bool handleMouseRelease(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        virtual bool handleMouseWheel(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

    protected:
        osg::Vec3d getIntersectedPoint(
            const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us,
            int mouseType);

        osg::Vec3d getIntersectedPoint(
            float x,float y, osgGA::GUIActionAdapter& us,
            int mouseType);

    private:
        osg::Vec3d mMouseDownPos;
        osg::Vec3d mCurMousePos;
        bool mIsMouseMove;
        osg::Vec3d mCurMouseIntersectPos;
        osg::Vec3d mMouseDownIntersectPos;
        double mDistanceToTarget;
        osg::Vec3d mMoveDirection;
        osg::Vec3d mMouseDragPos;
        int mMouseType;
    public:
        CSceneViewer* SceneViewer;
    };

    class VR_LOCAL_ENGINE_EXPORT CSceneViewer : public I3DViewer 
    {
    public:
        CSceneViewer(QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

        virtual ~CSceneViewer();

        virtual void initial();
		
        virtual void updateStateText();

        virtual void resizeGL(int width, int height);

        void updateMouseIcon(const double& x, const double& y,const bool& visible);
    protected:
        osg::ref_ptr<osg::Camera> mpHudCamera;
        osg::ref_ptr<osg::Node> mpStateNode;
        osg::ref_ptr<osgText::Text> mpStateText;
    };
}


#endif //_SCENEVIEWER_H