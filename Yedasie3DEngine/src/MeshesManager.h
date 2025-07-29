#pragma once

#include "Utils.h"
#include "OpenGLMesh.h"

class MeshesManager
{
public:
    static void Create();
    static void Destroy();

    MeshesManager();
	~MeshesManager();
	
    void            AddMesh(Mesh *mesh, std::string meshId);

    Mesh*           GetMesh(std::string meshId);

    UINT32          GetMeshesCount() { return mMeshes.size(); }

protected:
private:

    std::unordered_map<std::string, Mesh *>     mMeshes;
};

extern MeshesManager* gMeshesManager;