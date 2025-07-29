#pragma once

#include "Utils.h"
#include "Camera.h"

class CamerasManager
{
public:
    static void Create();
    static void Destroy();

    CamerasManager();
	~CamerasManager();
	
    void            AddCamera(Camera *camera, std::string cameraId, bool setAsActive = true);

    Camera*         GetCamera(std::string cameraId);
    Camera*         GetActiveCamera();

    void            SetActiveCamera(std::string cameraId);

    UINT32          GetCamerasCount() { return (UINT32)mCameras.size(); }

protected:
private:

    std::string                mActiveCameraId;
    std::unordered_map<std::string, Camera *>     mCameras;
};

extern CamerasManager* gCamerasManager;