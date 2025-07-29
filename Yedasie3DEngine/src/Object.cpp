#include "Object.h"

Object* gScene = nullptr;

Object::Object(std::string id, Mesh *mesh, Material *material, Texture *texture, bool visible)
{
    mId = id;
    mDrawable = true;
    mMesh = mesh;
    mMaterial = material;
    mTexture = texture;
    mVisible = visible;
    mTransform.SetPos(Vector3f(RNG::Float() * 300.f, RNG::Float() * 100.f, RNG::Float() * 300.f));
    mTransform.SetScale(Vector3f(2.f, 2.f, 2.f));
    if (!gScene)
    {
        mDrawable = false;
        id = "root";
        gScene = this;
    }
}

Object::~Object()
{
}

void Object::Update(float dtime)
{

}

void Object::DrawImGui()
{

}

void Object::Draw()
{
    if (!mVisible)
        return;

    if(mDrawable && IsInFrustum())
        DrawCurrentObject();

    for (const auto& obj : mChildren)
        obj->Draw();
}

void Object::DrawCurrentObject()
{
    UINT32 programId = gShadersManager->GetActiveShader()->GetProgramId();
    glUseProgram(programId);
    int x = glGetAttribLocation(programId, "a_Position");
    int y = glGetAttribLocation(programId, "a_Normal");
    int z = glGetAttribLocation(programId, "a_TexCoords");
    INT32 modelMatrixLocation = glGetUniformLocation(programId, "u_ModelMatrix");
    INT32 viewMatrixLocation = glGetUniformLocation(programId, "u_ViewMatrix");
    INT32 projectionLocation = glGetUniformLocation(programId, "u_ProjectionMatrix");
    INT32 lightPosLocation = glGetUniformLocation(programId, "u_LightPosition");
    INT32 eyePosLocation = glGetUniformLocation(programId, "u_EyePosition");
    INT32 matAmbientalLocation = glGetUniformLocation(programId, "u_Ka");
    INT32 matDifusseLocation = glGetUniformLocation(programId, "u_Kd");
    INT32 matSpecularLocation = glGetUniformLocation(programId, "u_Ks");
    INT32 matShininessLocation = glGetUniformLocation(programId, "u_Shininess");
    INT32 textureLocation = glGetUniformLocation(programId, "u_Texture");

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mTransform.GetModel()[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &gCamerasManager->GetActiveCamera()->GetViewMatrix()[0][0]);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &gCamerasManager->GetActiveCamera()->GetProjectionMatrix()[0][0]);

    glUniform3f(lightPosLocation,       0.f, 150.f, 0.f);
    Vector3f camPos = gCamerasManager->GetActiveCamera()->GetPosition();
    glUniform3fv(eyePosLocation, 1,         &(camPos.x));
    glUniform3fv(matAmbientalLocation, 1,   mMaterial->GetAmbientalRef());
    glUniform3fv(matDifusseLocation, 1,     mMaterial->GetDiffuseRef());
    glUniform3fv(matSpecularLocation, 1,    mMaterial->GetSpecularRef());
    glUniform1f(matShininessLocation,       mMaterial->GetShininess());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->mTextureId);
    glUniform1i(textureLocation, 0);

    mMesh->DrawMesh();
}

void Object::AddChild(std::shared_ptr<Object> child)
{
    mChildren.push_back(child);
}

bool Object::IsInFrustum()
{
    auto camera = gCamerasManager->GetActiveCamera();

    bool res = true;

    float th = mMesh->GetSphereRadius();

    Vector4f screenPos = camera->GetProjectionMatrix() * camera->GetViewMatrix() * mTransform.GetModel() * Vector4f(mMesh->GetSphereCenter(), 1.f);
    res = abs(screenPos.x) - th < screenPos.w &&
        abs(screenPos.y) - th < screenPos.w &&
        -th < screenPos.z &&
        screenPos.z - th < screenPos.w;

    return res;
}
