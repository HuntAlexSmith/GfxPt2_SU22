//*****************************************************************************
//	File:   ImGUISystem.cpp
//  Author: Hunter Smith
//  Date:   06/13/2022
//  Description: ImGUI System, which will manage everything ImGUI is doing
//*****************************************************************************

#include "Engine.h"
#include "WindowSystem.h"
#include "ImGUISystem.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

ImGUISystem::ImGUISystem() : System(ImGUISys)
{

}

void ImGUISystem::Initialize()
{
	// Check the ImGUI version first
	IMGUI_CHECKVERSION();

	// On initialization, create an ImGUI context
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Get the window system for window handle and opengl context
	WindowSystem* windowSys = dynamic_cast<WindowSystem*>(GetParent()->GetSystem(WindowSys));
	SDL_Window* handle = nullptr;
	SDL_GLContext glCont = nullptr;
	if (windowSys)
	{
		handle = windowSys->GetWindowHandle();
		glCont = windowSys->GetGLContext();
	}

	// Initialize ImGUI with SDL
	ImGui_ImplSDL2_InitForOpenGL(handle, glCont);

	// Initialize OpenGL with ImGUI
	ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGUISystem::Update(float dt)
{
	// Start a new frame of ImGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Print something
	ImGui::Begin("My First Window :)");
	ImGui::Text("Hello World!");
	ImGui::End();

	// Now to get it rendered
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUISystem::Shutdown()
{
	// Shutdown OpenGL first, then SDL, then ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGUISystem::ProcessEvents(const SDL_Event* sdlEvent)
{
	ImGui_ImplSDL2_ProcessEvent(sdlEvent);
}

ImGUISystem::~ImGUISystem()
{

}