#pragma once
#include "Utils.h"
#include "ObjLoader.h"

#define MESHES_DIR "../Resources/3dModels/"

enum OGL_BUFFER
{
    OGL_BUFF_VERTICES,
    OGL_BUFF_NORMALS,
    OGL_BUFF_TEXCOORDS,
    OGL_BUFF_INDICES,
    OGL_BUFF_COUNT
};

class Mesh
{
public:
    Mesh(const std::string& fileName);

    void DrawMesh();
    virtual ~Mesh();

    Vector3f GetSphereCenter() { return mSphereCenter; }
    float    GetSphereRadius() { return mSphereRadius; }

protected:
private:

    void CalcBoundingSphere();

    GLuint                      mVertexArrayBuffers[OGL_BUFF_COUNT];
    GLuint						mVertexArrayObject;

    std::vector<Vector3f>       mVertices;
    std::vector<Vector3f>       mNormals;
    std::vector<Vector2f>       mTexCoords;
    std::vector<UINT32>         mIndices;

    Vector3f                    mSphereCenter;
    float                       mSphereRadius;
};

