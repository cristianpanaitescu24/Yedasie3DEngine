#pragma once

#include "Utils.h"
#include "OpenGLShader.h"

class ShadersManager
{
public:
    static void Create();
    static void Destroy();

    ShadersManager();
	~ShadersManager();
	
    void            AddShader(Shader *shader, std::string id);

    Shader*         GetShader(std::string shaderId);
    Shader*         GetActiveShader();

    void            SetActiveShader(std::string shaderId);

    UINT32          GetShadersCount() { return (UINT32)mShaders.size(); }

protected:
private:

    std::string                mActiveShaderId;
    std::unordered_map<std::string, Shader *>     mShaders;
};

extern ShadersManager* gShadersManager;