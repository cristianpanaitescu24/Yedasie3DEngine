#pragma once
#include "Utils.h"

#define TEXTURES_DIR "../../Resources/Images/"

class Texture
{
public:
    Texture(const std::string& fileName, UINT32 format = 0);
	Texture(UINT32 format, UINT32 width, UINT32 height);

	virtual ~Texture();

    GLuint mTextureId;
    int   mWidth;
    int   mHeight;

protected:
private:


};

