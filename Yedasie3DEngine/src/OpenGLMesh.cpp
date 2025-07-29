#include "OpenGLMesh.h"

Mesh::Mesh(const std::string& fileName)
{
    OBJModel model((MESHES_DIR + fileName).c_str());
    IndexedModel& indexedModel = model.ToIndexedModel();

    indexedModel.CalcNormals();

    mVertices.assign (indexedModel.positions.begin(), indexedModel.positions.end());
    mNormals.assign  (indexedModel.normals.begin(),   indexedModel.normals.end());
    mTexCoords.assign(indexedModel.texCoords.begin(), indexedModel.texCoords.end());
    mIndices.assign  (indexedModel.indices.begin(),   indexedModel.indices.end());

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(OGL_BUFF_COUNT, mVertexArrayBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[OGL_BUFF_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vector3f), mVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[OGL_BUFF_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(Vector3f), mNormals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffers[OGL_BUFF_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(Vector2f), mTexCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffers[OGL_BUFF_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(UINT32), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    CalcBoundingSphere();
}


Mesh::~Mesh()
{
    glDeleteBuffers(OGL_BUFF_COUNT, mVertexArrayBuffers);
    glDeleteBuffers(1, &mVertexArrayObject);
}

void Mesh::DrawMesh()
{
    glBindVertexArray(mVertexArrayObject);
    glDrawElementsBaseVertex(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0, 0);
    glBindVertexArray(0);
}

void Mesh::CalcBoundingSphere()
{
    Vector3f avg = Vector3f(0.f, 0.f, 0.f);

    for (auto vert : mVertices)
    {
        avg += vert;
    }

    mSphereCenter = avg / Vector3f(mVertices.size());
    mSphereRadius = 0.f;

    for (auto vert : mVertices)
    {
        auto dist = glm::distance(mSphereCenter, vert);
        if(mSphereRadius < dist)
            mSphereRadius = dist;
    }

    int k = 0;
}