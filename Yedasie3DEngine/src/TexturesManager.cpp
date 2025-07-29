#include "TexturesManager.h"

TexturesManager* gTexturesManager = nullptr;


void TexturesManager::Create()
{
    gTexturesManager = new TexturesManager();
}

void TexturesManager::Destroy()
{
    ASSERT(gTexturesManager);
    delete gTexturesManager;
}

TexturesManager::TexturesManager()
{
    ASSERT(!gTexturesManager);
}

TexturesManager::~TexturesManager()
{
    for (std::pair<std::string, Texture *> it : mTextures)
        delete it.second;
    mTextures.clear();
}

void TexturesManager::AddTexture(Texture *texture, std::string id)
{
    ASSERT(mTextures.find(id) == mTextures.end());

    if (texture)
    {
        mTextures[id] = texture;
    }
}

Texture* TexturesManager::GetTexture(std::string id)
{
    ASSERT(mTextures.find(id) != mTextures.end());

    return mTextures[id];
}