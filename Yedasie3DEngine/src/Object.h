#pragma once

#include "Utils.h"
#include "SceneManager.h"
#include "Transform.h"

class Object
{
public:
    Object(std::string id = "_", Mesh *mesh = nullptr, Material *material = nullptr, Texture *texture = nullptr, bool visible = true);
    virtual ~Object();
    
    void Update(float dtime);
    void DrawImGui();
    void Draw();

    void AddChild(std::shared_ptr<Object> child);

private:
    void DrawCurrentObject();
    bool IsInFrustum();

    std::string      mId;
    bool             mVisible;
    bool             mDrawable;
    Mesh            *mMesh;
    Material        *mMaterial;
    Texture         *mTexture;
    Transform        mTransform;

    std::vector<std::shared_ptr<Object>> mChildren;
};

extern Object* gScene;