#pragma once

#include <SDL.h>
#include "Utils.h"

class App
{
public:
    App();
    ~App();

	void            Run();

private:

	STATUS            Init();

	SDL_Window		*mSDLWindow;
	SDL_GLContext	 mSDLGLContext;
	ImGuiContext	*mImGuiContext;

    ImGuiIO         mImGuiIO;
    bool            mRunning;
};