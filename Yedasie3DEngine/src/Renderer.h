#pragma once
#include "Utils.h"
#include "Object.h"
#include "SceneManager.h"
#include "imgui/imgui_utils.h"

#define DISPLAY_WIDTH 1920
#define DISPLAY_HEIGHT 1000

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    void OnKeyDown(int key);
    void OnKeyUp(int key);

    virtual void DrawImgui(float dtime);
    virtual void Update(float dtime);
    virtual void Draw();

    Texture    *GetOutputTexture();

protected:
private:

    void        UpdateViewport(ImVec2 vpSize);
    void        BindFBOforRendering();

    Texture     *mOutputTexture;
    UINT32      mFramebuffer;
    UINT32      mRenderbuffer;

	float		mTime;

    // Viewport size
    UINT32      mVPWidth;
    UINT32      mVPHeight;

    ImVec2      mOldMousePos;

    std::shared_ptr<Object>      mSceneRoot;

    ImGuiRenderer   *mImGuiRenderer;
};

