#pragma once

#include "Utils.h"

class Shader
{
public:
	Shader(const std::string vertexCode, const std::string fragmentCode);

    GLuint 				GetProgramId() { return mProgramId; }
	
    virtual ~Shader();
protected:
private:

	static const unsigned int NUM_SHADERS = 2;

	std::string			LoadShader(const std::string& fileName);
	void				CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint				CreateShader(const std::string& text, unsigned int type);

	GLuint				mProgramId;
};

