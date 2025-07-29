#pragma once

#include "Utils.h"
#include "tinyxml2.h"

#include "CamerasManager.h"
#include "ShadersManager.h"
#include "MeshesManager.h"
#include "MaterialsManager.h"
#include "TexturesManager.h"
#include "SceneManager.h"
#include "Object.h"

class Object;

class SceneManager
{
public:
    static void Create();
    static void Destroy();

    SceneManager();
    ~SceneManager();

    STATUS LoadScene(std::string sceneFileName);
    STATUS SaveScene(std::string sceneFileName, bool overwrite = true);

protected:
private:

    void LoadSceneShaders(tinyxml2::XMLElement *elem);
    void LoadSceneCameras(tinyxml2::XMLElement *elem);
    void LoadSceneMeshes(tinyxml2::XMLElement *elem);
    void LoadSceneMaterials(tinyxml2::XMLElement *elem);
    void LoadSceneTextures(tinyxml2::XMLElement *elem);

    void LoadSceneObjects(tinyxml2::XMLElement *elem, Object* parent);
};

extern SceneManager* gSceneManager;