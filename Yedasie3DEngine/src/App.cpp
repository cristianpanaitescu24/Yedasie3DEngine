#include "App.h"
#include "Renderer.h"

#define WINDOW_TITILE "Yedasie Engine"
#define OPENGL_VERSION "#version 430"

App::App()
{
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(mSDLGLContext);
    SDL_DestroyWindow(mSDLWindow);
    SDL_Quit();
}

STATUS App::Init()
{
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS | SDL_INIT_SENSOR | SDL_INIT_CAMERA);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mSDLWindow = SDL_CreateWindow(WINDOW_TITILE, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	mSDLGLContext = SDL_GL_CreateContext(mSDLWindow);
    SDL_GL_SetSwapInterval(1); // Enable vsync
	mImGuiContext = ImGui::CreateContext();
    //ImGuiKey
	glewExperimental = true;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    mImGuiIO = ImGui::GetIO();

    bool st = ImGui_ImplSDL3_InitForOpenGL(mSDLWindow, mSDLGLContext);
	if (st != true)
	{
		SDL_DestroyWindow(mSDLWindow);
        SDL_GL_DestroyContext(mSDLGLContext);
		ImGui::DestroyContext(mImGuiContext);

		return ST_FAIL;
	}
    
    ImGui_ImplOpenGL3_Init(OPENGL_VERSION);
    ImGui::StyleColorsDark();

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);

    return ST_OK;
}

void App::Run()
{
	Init();

    SDL_Event e;
    UINT64 oldTime = SDL_GetTicks();
    float timer = 0;
    int frames = 0;
    Renderer sceneRenderer;

	mRunning = true;
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (mRunning)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT
                )
                mRunning = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                ImGui::SetWindowFocus();
            io.WantCaptureKeyboard = true;
			ImGui_ImplSDL3_ProcessEvent(&e);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        UINT64 timeNow = SDL_GetTicks();
        UINT64 dtime = timeNow - oldTime;
        oldTime = timeNow;

        timer += dtime;
        if (timer > 1000)
        {
            printf("FPS: %d\n", frames);
            timer = 0;
            frames = 1;
        }
        else
        {
            frames++;
        }

        sceneRenderer.Draw();

        ImGui::Begin("Camera View", NULL);
        {
            if (ImGui::IsWindowFocused())
            {
                sceneRenderer.Update(float(dtime));
                if (ImGui::IsKeyDown(ImGuiKey_Space))
                    SDL_WarpMouseInWindow(mSDLWindow, (INT32)ImGui::GetWindowWidth() / 2, (INT32)ImGui::GetWindowHeight() / 2);
                SDL_HideCursor();
            }
            else
            {
                SDL_ShowCursor();
            }

            auto tex = sceneRenderer.GetOutputTexture();
            ImGui::Image((ImTextureID)(tex->mTextureId), ImVec2((float)tex->mWidth, (float)tex->mHeight), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
        }
        ImGui::End();

        sceneRenderer.DrawImgui(float(dtime));

        ImGui::Render();

        SDL_GL_MakeCurrent(mSDLWindow, mSDLGLContext);
        glViewport(0, 0, (int)mImGuiIO.DisplaySize.x, (int)mImGuiIO.DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(mSDLWindow);
    }
}