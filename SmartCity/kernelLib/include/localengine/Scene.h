/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#ifndef _SCENE_H
#define _SCENE_H

#include "localengine/EngineConfig.h"
#include "localengine/I3DViewer.h"

#define CS_3DVIEWER "CS_3DVIEWER"
#define CS_IMAGEVIEWER "CS_IMAGEVIEWER"
#define HD_3DVIEWER "HD_3DVIEWER"
#define HD_IMAGEVIEWER "HD_IMAGEVIEWER"
#define PRE_VIEWER "PRE_VIEWER"

namespace Engine
{
    class VR_LOCAL_ENGINE_EXPORT  CScene 
    {
    public:
        CScene();

        virtual ~CScene();

        static CScene* instance();
        
        I3DViewer* getViewer(const std::string& in_id);

    };
}

#endif //_SCENE_H