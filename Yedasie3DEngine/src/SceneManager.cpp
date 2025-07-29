#include "SceneManager.h"

SceneManager* gSceneManager = nullptr;

void SceneManager::Create()
{
    gSceneManager = new SceneManager();
}

void SceneManager::Destroy()
{
    ASSERT(gSceneManager);
    delete gSceneManager;
}

SceneManager::SceneManager()
{
    ASSERT(!gSceneManager);
}

SceneManager::~SceneManager()
{
}

void PrintElems(tinyxml2::XMLElement *elem, int level)
{
    char spaces[100] = "";
    for (int i = 0; i < level; i++)
    {
        spaces[i * 4 + 0] = ' ';
        spaces[i * 4 + 1] = ' ';
        spaces[i * 4 + 2] = ' ';
        spaces[i * 4 + 3] = ' ';
    }
    spaces[level*4] = NULL;

    for (auto e = elem->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        printf("%s%s\n", spaces, e->Name());
        PrintElems(e, level + 1);
    }
}

STATUS SceneManager::SaveScene(std::string sceneFileName, bool overwrite)
{
    return ST_OK;
}

void SceneManager::LoadSceneShaders(tinyxml2::XMLElement *elem)
{
    for (auto e = elem->FirstChildElement("shader"); e != nullptr; e = e->NextSiblingElement("shader"))
    {
        Shader *shader = new Shader(e->Attribute("vert-file"), e->Attribute("frag-file"));
        gShadersManager->AddShader(shader, e->Attribute("id"));
    }
}

void SceneManager::LoadSceneCameras(tinyxml2::XMLElement *elem)
{
    for (auto e = elem->FirstChildElement("camera"); e != nullptr; e = e->NextSiblingElement("camera"))
    {
        Camera *camera = NULL;

        Vector3f cameraPos;
        auto positionElem = e->FirstChildElement("position");
        cameraPos.x = positionElem->FloatAttribute("x");
        cameraPos.y = positionElem->FloatAttribute("y");
        cameraPos.z = positionElem->FloatAttribute("z");

        if (strcmp(e->Attribute("type"), "perspective") == 0)
        {
            auto perspectiveElem = e->FirstChildElement("perspective");
            camera = new Camera(cameraPos,
                perspectiveElem->FloatAttribute("fov"),
                perspectiveElem->FloatAttribute("aspect"),
                perspectiveElem->FloatAttribute("near"),
                perspectiveElem->FloatAttribute("far"));
        }
        else if (strcmp(e->Attribute("type"), "ortho") == 0)
        {
            // TODO: NOT IMPLEMENTED
        }

        if (camera)
        {
            gCamerasManager->AddCamera(camera, e->Attribute("id"));
        }
    }
}

void SceneManager::LoadSceneMeshes(tinyxml2::XMLElement *elem)
{
    for (auto e = elem->FirstChildElement("mesh"); e != nullptr; e = e->NextSiblingElement("mesh"))
    {
        Mesh *mesh = new Mesh(e->Attribute("file"));
        gMeshesManager->AddMesh(mesh, e->Attribute("id"));
    }
}

void SceneManager::LoadSceneMaterials(tinyxml2::XMLElement *elem)
{
    for (auto e = elem->FirstChildElement("material"); e != nullptr; e = e->NextSiblingElement("material"))
    {
        Material *material = new Material(e->Attribute("file"));
        gMaterialsManager->AddMaterial(material, e->Attribute("id"));
    }
}

void SceneManager::LoadSceneTextures(tinyxml2::XMLElement *elem)
{
    for (auto e = elem->FirstChildElement("texture"); e != nullptr; e = e->NextSiblingElement("texture"))
    {
        Texture *texture = new Texture(e->Attribute("file"));
        gTexturesManager->AddTexture(texture, e->Attribute("id"));
    }
}

void SceneManager::LoadSceneObjects(tinyxml2::XMLElement *elem, Object* parent)
{
    for (auto e = elem->FirstChildElement("object"); e != nullptr; e = e->NextSiblingElement("object"))
    {
        auto obj = std::make_shared<Object>(
            e->Attribute("id"),
            gMeshesManager->GetMesh(e->Attribute("mesh-id")),
            gMaterialsManager->GetMaterial(e->Attribute("material-id")),
            gTexturesManager->GetTexture(e->Attribute("texture-id")));

        parent->AddChild(obj);

        tinyxml2::XMLElement* childrenElem = e->FirstChildElement("children");
        if (childrenElem)
            LoadSceneObjects(childrenElem, obj.get());
    }
}


STATUS SceneManager::LoadScene(std::string sceneFileName)
{
    tinyxml2::XMLDocument sceneXmlDoc;

    tinyxml2::XMLError eResult = sceneXmlDoc.LoadFile(sceneFileName.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) return ST_INVALID_ARGS;

    tinyxml2::XMLElement* root = sceneXmlDoc.FirstChildElement("scene");
    if (root == nullptr) return ST_FAIL;

    PrintElems(root, 0);

    tinyxml2::XMLElement* shadersElem = root->FirstChildElement("shaders");
    if (shadersElem != nullptr)
        LoadSceneShaders(shadersElem);
    else
        return ST_FAIL;

    tinyxml2::XMLElement* camerasElem = root->FirstChildElement("cameras");
    if (camerasElem != nullptr)
        LoadSceneCameras(camerasElem);
    else
        return ST_FAIL;

    tinyxml2::XMLElement* meshesElem = root->FirstChildElement("meshes");
    if (meshesElem != nullptr)
        LoadSceneMeshes(meshesElem);
    else
        return ST_FAIL;

    tinyxml2::XMLElement* materialsElem = root->FirstChildElement("materials");
    if (materialsElem != nullptr)
        LoadSceneMaterials(materialsElem);
    else
        return ST_FAIL;

    tinyxml2::XMLElement* texturesElem = root->FirstChildElement("textures");
    if (texturesElem != nullptr)
        LoadSceneTextures(texturesElem);
    else
        return ST_FAIL;

    tinyxml2::XMLElement* objectsElem = root->FirstChildElement("scene-root");
    if (objectsElem != nullptr)
        LoadSceneObjects(objectsElem, gScene);
    else
        return ST_FAIL;

    return ST_OK;
}
