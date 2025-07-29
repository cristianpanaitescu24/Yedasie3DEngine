#include "Renderer.h"

Renderer::Renderer()
{
    mTime = 0.f;

    SceneManager::Create();
    CamerasManager::Create();
    ShadersManager::Create();
    MeshesManager::Create();
    MaterialsManager::Create();
    TexturesManager::Create();

    mSceneRoot = std::make_shared<Object>();

    gSceneManager->LoadScene("../Resources/Scenes/scene1.xml");
    mVPWidth = mVPHeight = 0;

    mOutputTexture = new Texture(GL_RGBA, 500, 500);
    glGenFramebuffers(1, &mFramebuffer);
    glGenRenderbuffers(1, &mRenderbuffer);

    mImGuiRenderer = new ImGuiRenderer();
}

Renderer::~Renderer()
{
    SceneManager::Destroy();
    CamerasManager::Destroy();
    ShadersManager::Destroy();
    MeshesManager::Destroy();
    MaterialsManager::Destroy();
    TexturesManager::Destroy();

    glDeleteFramebuffers(1, &mRenderbuffer);
    glDeleteFramebuffers(1, &mFramebuffer);

    delete mImGuiRenderer;
}

void Renderer::DrawImgui(float dtime)
{

    mImGuiRenderer->RenderUI();
}

void Renderer::UpdateViewport(ImVec2 vpSize)
{
    if (vpSize.x != mVPWidth || vpSize.y != mVPHeight)
    {
        mVPWidth = (UINT32)vpSize.x;
        mVPHeight = (UINT32)vpSize.y;
        gCamerasManager->GetActiveCamera()->UpdateAspectRatio(vpSize.x / vpSize.y);

        delete(mOutputTexture);
        mOutputTexture = new Texture(GL_RGBA, mVPWidth, mVPHeight);
    }
}

void Renderer::Update(float dtime)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.KeysDown[32])
        return;

    mTime += dtime * 0.001f;
    
    auto winSize = ImGui::GetWindowSize();
    UpdateViewport(winSize);

    float rotateSpeed = dtime * 0.0025f;

    gCamerasManager->GetActiveCamera()->RotateX(-(mOldMousePos.y - io.MousePos.y) * rotateSpeed);
    gCamerasManager->GetActiveCamera()->RotateY((mOldMousePos.x - io.MousePos.x) * rotateSpeed);

    mOldMousePos.x = (float)((INT32)winSize.x / 2);
    mOldMousePos.y = (float)((INT32)winSize.y / 2);

    float moveDist = dtime * 0.18f;

    if (io.KeysDown['a'])
        gCamerasManager->GetActiveCamera()->MoveRight(moveDist);
    if (io.KeysDown['d'])
        gCamerasManager->GetActiveCamera()->MoveRight(-moveDist);
    if (io.KeysDown['w'])
        gCamerasManager->GetActiveCamera()->MoveForward(moveDist);
    if (io.KeysDown['s'])
        gCamerasManager->GetActiveCamera()->MoveForward(-moveDist);
}

void Renderer::Draw()
{
    gShadersManager->SetActiveShader("basic-shader");

    BindFBOforRendering();

    glViewport(0, 0, mVPWidth, mVPHeight);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSceneRoot->Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::OnKeyDown(int key)
{}

void Renderer::OnKeyUp(int key)
{}

Texture* Renderer::GetOutputTexture()
{
    return mOutputTexture;
}

void Renderer::BindFBOforRendering()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mVPWidth, mVPHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderbuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mOutputTexture->mTextureId, 0);
}