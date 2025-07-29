#include "ShadersManager.h"

ShadersManager* gShadersManager = nullptr;

void ShadersManager::Create()
{
    gShadersManager = new ShadersManager();
}

void ShadersManager::Destroy()
{
    ASSERT(gShadersManager);
    delete gShadersManager;
}


ShadersManager::ShadersManager()
{
    ASSERT(!gShadersManager);
}

ShadersManager::~ShadersManager()
{
    for (std::pair<std::string, Shader *> it : mShaders)
        delete it.second;
    mShaders.clear();
}

void ShadersManager::AddShader(Shader *shader, std::string id)
{
    ASSERT(mShaders.find(id) == mShaders.end());

    if (shader)
    {
        mShaders[id] = shader;
    }
}

Shader* ShadersManager::GetShader(std::string shaderId)
{
    ASSERT(mShaders.find(shaderId) != mShaders.end());

    return mShaders[shaderId];
}

Shader* ShadersManager::GetActiveShader()
{
    return GetShader(mActiveShaderId);
}

void ShadersManager::SetActiveShader(std::string shaderId)
{
    ASSERT(mShaders.find(shaderId) != mShaders.end());
    mActiveShaderId = shaderId;
}
