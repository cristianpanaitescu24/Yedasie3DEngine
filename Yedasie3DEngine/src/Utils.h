#pragma once

#include "imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glew.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include <SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <memory>
#include "RNG.h"

#include "tinyxml2.h"

typedef glm::vec4 Vector4f;
typedef glm::vec3 Vector3f;
typedef glm::vec2 Vector2f;

typedef          int  INT32;
typedef unsigned int UINT32;
typedef unsigned char UINT8;

typedef UINT32 STATUS;

#define ST_OK					0
#define ST_FAIL					-1
#define ST_NOT_IMPLEMENTED		-2
#define ST_INVALID_ARGS			-3

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif


#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define DEBUG_BREAK __debugbreak()

#ifdef _DEBUG
#define GLCall(c) \
GLClearErrors(); \
c; \
GLCheckErrors(#c, __FILENAME__, __LINE__);
#else
#define GLCall(c) c;
#endif
static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckErrors(const char * functionCall, const char *file, int line)
{
	while (int glerr = glGetError())
	{
		printf("OpenGL error: %d at call '%s' in file: %s, line: %d\n", glerr, functionCall, file, line);
#ifdef DEBUG_BREAK
		DEBUG_BREAK;
#endif
	}
}

static void GetGLError(const char *errMessage)
{
	//int glerr = glGetError();
	//while (glerr != GL_NO_ERROR)
	//{
	//	printf("OpenGL error %d: %s\n", glerr, errMessage);
	//	glerr = glGetError();
	//}
}
