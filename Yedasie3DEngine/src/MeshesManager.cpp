#include "MeshesManager.h"

MeshesManager* gMeshesManager = nullptr;

void MeshesManager::Create()
{
    gMeshesManager = new MeshesManager();
}

void MeshesManager::Destroy()
{
    ASSERT(gMeshesManager);
    delete gMeshesManager;
}

MeshesManager::MeshesManager()
{
    ASSERT(!gMeshesManager);
}

MeshesManager::~MeshesManager()
{
    for (std::pair<std::string, Mesh *> it : mMeshes)
        delete it.second;
    mMeshes.clear();
}

void MeshesManager::AddMesh(Mesh *mesh, std::string meshId)
{
    ASSERT(mMeshes.find(meshId) == mMeshes.end());

    if (mesh)
    {
        mMeshes[meshId] = mesh;
    }
}

Mesh* MeshesManager::GetMesh(std::string meshId)
{
    ASSERT(mMeshes.find(meshId) != mMeshes.end());

    return mMeshes[meshId];
}