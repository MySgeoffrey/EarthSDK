/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#ifndef _IMAGEVIEWER_H
#define _IMAGEVIEWER_H

#include "localengine/I3DViewer.h"
#include "osgGA/TrackballManipulator"

namespace Engine
{
    class VR_LOCAL_ENGINE_EXPORT CImageViewerManipulator : public osgGA::TrackballManipulator
    {
    public:
        CImageViewerManipulator();

        virtual ~CImageViewerManipulator();

        virtual void rotateTrackball(const float px0, const float py0,
            const float px1, const float py1, const float scale);

        virtual void rotateWithFixedVertical(const float dx, const float dy);

        virtual void rotateWithFixedVertical(const float dx, const float dy, const osg::Vec3f& up);
    };

    class VR_LOCAL_ENGINE_EXPORT  CImageViewer : public I3DViewer 
    {
    public:
        CImageViewer(QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

        virtual ~CImageViewer();

        virtual void initial(); 

    };
}

#endif //_IMAGEVIEWER_H