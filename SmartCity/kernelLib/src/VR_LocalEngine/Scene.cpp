/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#include "localengine/Scene.h"

/**
 * Scene implementation
 */

namespace Engine
{
    static CScene* g_pScene = new CScene();
    CScene::CScene()
    {

    }

    CScene::~CScene()
    {

    }

    CScene* CScene::instance()
    {
        return g_pScene;
    }

    I3DViewer* CScene::getViewer(const std::string& in_id)
    {
        I3DViewer* pViewer = NULL;
        return pViewer;
    }
}