#include "CamerasManager.h"

CamerasManager* gCamerasManager = nullptr;

void CamerasManager::Create()
{
    gCamerasManager = new CamerasManager();
}

void CamerasManager::Destroy()
{
    ASSERT(gCamerasManager);
    delete gCamerasManager;
}


CamerasManager::CamerasManager()
{
    ASSERT(!gCamerasManager);
}

CamerasManager::~CamerasManager()
{
    for (std::pair<std::string, Camera *> it : mCameras)
        delete it.second;
    mCameras.clear();
}

void CamerasManager::AddCamera(Camera *camera, std::string cameraId, bool setAsActive)
{
    ASSERT(mCameras.find(cameraId) == mCameras.end());

    if (camera)
    {
        mCameras[cameraId] = camera;
        if (setAsActive)
        {
            mActiveCameraId = cameraId;
        }
    }
}

Camera* CamerasManager::GetCamera(std::string cameraId)
{
    ASSERT(mCameras.find(cameraId) != mCameras.end());

    return mCameras[cameraId];
}

Camera* CamerasManager::GetActiveCamera()
{
    return GetCamera(mActiveCameraId);
}


void CamerasManager::SetActiveCamera(std::string cameraId)
{
    ASSERT(mCameras.find(cameraId) != mCameras.end());
    mActiveCameraId = cameraId;
}
