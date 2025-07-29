#pragma once

#include "Utils.h"
#include "OpenGLTexture.h"

class TexturesManager
{
public:
    static void Create();
    static void Destroy();

    TexturesManager();
	~TexturesManager();
	
    void            AddTexture(Texture *texture, std::string id);

    Texture*        GetTexture(std::string id);

    UINT32          GetTexturesCount() { return mTextures.size(); }

protected:
private:

    std::unordered_map<std::string, Texture *>     mTextures;
};

extern TexturesManager* gTexturesManager;