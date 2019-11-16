#ifndef _I3DVIEWER_H
#define _I3DVIEWER_H
#include <osg/ArgumentParser>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtOpenGL/QGLWidget>
#include <iostream>
#include "localengine/EngineConfig.h"
#include <osg/AutoTransform>

namespace Engine
{
    class VR_LOCAL_ENGINE_EXPORT IAdapterWidget :public QGLWidget
    {
    public:
        IAdapterWidget(QWidget *parent = 0, const char* name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

        virtual ~IAdapterWidget()
        {

        }

        osgViewer::GraphicsWindow* getGraphicsWindow()
        {
            return _gw.get();
        }

        const osgViewer::GraphicsWindow* getGraphicsWidow()const
        {
            return _gw.get();
        }
    protected:
        virtual void resizeGL(int width, int height);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseDoubleClickEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);//
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual void wheelEvent(QWheelEvent * event);

        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;

    };

    class VR_LOCAL_ENGINE_EXPORT I3DViewer
        :public osgViewer::Viewer, public IAdapterWidget
    {
    public:
        I3DViewer(QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

        virtual ~I3DViewer();

        virtual void paintGL();

        osg::ref_ptr<osg::Group> getRoot();

        virtual void initial();

        virtual void updateAxisPosition(const osg::Vec3d& in_pos);

        osg::Node* createAxis();

        void setVisible(const bool& in_visible);

    protected:
        osg::Node* createHUDText();

        osg::Node* createHUDTextEx(const float &x, const float &y, const float &width,
            const float &height, const float &size, const std::string &content, const osg::Vec4 &color, const std::string &font);

        osg::Node* createHUDImage(const float &x, const float &y, const float &width,const float &height);
    protected:
        QTimer mTimer;
        bool mFrameState;
        osg::ref_ptr<osg::Group> mpRoot;
        osg::ref_ptr<osg::AutoTransform> mpAxisNode;
    };
}
#endif