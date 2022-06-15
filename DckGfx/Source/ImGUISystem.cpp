//*****************************************************************************
//	File:   ImGUISystem.cpp
//  Author: Hunter Smith
//  Date:   06/13/2022
//  Description: ImGUI System, which will manage everything ImGUI is doing
//*****************************************************************************

#include "Engine.h"
#include "WindowSystem.h"
#include "ObjectManagerSystem.h"
#include "ImGUISystem.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

ImGUISystem::ImGUISystem() : System(ImGUISys)
{

}

void ImGUISystem::Initialize()
{
	// Check the ImGUI version first
	IMGUI_CHECKVERSION();

	// On initialization, create an ImGUI context
	ImGui::CreateContext();

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

	// Call private functions here for rendering windows
	DebugWindow(dt);
	ObjectList();
	SelectedObject();

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

void ImGUISystem::DebugWindow(float dt)
{
	// Calculate current frame rate
	unsigned int frameRate = static_cast<unsigned int>(1.0f / dt);

	// Render the debug information window
	ImVec2 windowPos(0, 0);
	ImVec2 windowSize(250, 50);
	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	// Print something
	ImGui::Begin("Debug Info");
	ImGui::Text("Frame Rate: %u", frameRate);
	prevWindowPos = ImGui::GetWindowPos();
	prevWindowSize = ImGui::GetWindowSize();
	ImGui::End();
}

void ImGUISystem::ObjectList()
{
	// Get the object manager system to see how many objects there are
	ObjectManagerSystem* objManSys = dynamic_cast<ObjectManagerSystem*>(GetParent()->GetSystem(ObjectManagerSys));

	if (objManSys)
	{
		// Get the object list and count of objects
		RenderObject** objects = objManSys->GetAllObjects();
		int objectCount = objManSys->GetCount();

		// Calculate the size of the next window
		ImVec2 nextWindowPos(prevWindowPos.x, prevWindowPos.y + prevWindowSize.y);
		ImVec2 nextWindowSize(prevWindowSize.x, 200);
		ImGui::SetNextWindowPos(nextWindowPos);
		ImGui::SetNextWindowSize(nextWindowSize);

		// Start creating the window
		ImGui::Begin("Objects");
		for (int i = 0; i < objectCount; ++i)
		{
			std::string buttonLabel = "Object #" + std::to_string(i+1) + " " + objects[i]->GetName();
			if (objects[i] == currentObject_)
				buttonLabel += " (Selected)";
			if (ImGui::Button(buttonLabel.c_str()))
				currentObject_ = objects[i];
		}
		prevWindowPos = ImGui::GetWindowPos();
		prevWindowSize = ImGui::GetWindowSize();
		ImGui::End();
	}
}

void ImGUISystem::SelectedObject()
{
	if (currentObject_)
	{
		// Get the window system to figure out where this window will be
		WindowSystem* windowSys = dynamic_cast<WindowSystem*>(GetParent()->GetSystem(WindowSys));
		if (windowSys)
		{
			// Get the window dimensions
			int width, height;
			windowSys->GetWindowSize(&width, &height);

			// Set the window's position and height
			ImVec2 nextPos(width - 350, 0);
			ImVec2 nextSize(350, 400);
			ImGui::SetNextWindowPos(nextPos);
			ImGui::SetNextWindowSize(nextSize);

			// Start creating the window
			ImGui::Begin(currentObject_->GetName().c_str());

			// Start showing the position part of the object
			glm::vec4 objPos = currentObject_->GetPosition();
			bool modified = false;
			ImGui::Text("Position:");
			if (ImGui::DragFloat("PosX", &objPos.x, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;

			if (ImGui::DragFloat("PosY", &objPos.y, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;

			if (ImGui::DragFloat("PosZ", &objPos.z, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;

			if (modified)
			{
				currentObject_->SetPosition(objPos);
			}

			// Start showing the scale part of the object
			glm::vec3 objScale = currentObject_->GetScale();
			modified = false;
			ImGui::Text("Scale:");
			if (ImGui::DragFloat("ScaleX", &objScale.x, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;
			if (ImGui::DragFloat("ScaleY", &objScale.y, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;
			if (ImGui::DragFloat("ScaleZ", &objScale.z, 0.01f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;

			if (modified)
			{
				currentObject_->SetScale(objScale);
			}

			//
			//	WILL HANDLE ROTATION LATER. ROTATION, WITH HOW HANDLED, WILL PROBABLY NEED A DEBUG LINE
			//

			// Start showing the tint part of the object if the current mesh isn't a normal mesh
			glm::vec3 tint = currentObject_->GetTint();
			float colors[3] = { tint.r, tint.g, tint.b };

			ImGui::Text("Tint:");
			if (ImGui::ColorEdit3("Tint Color", colors))
			{
				glm::vec3 newTint(colors[0], colors[1], colors[2]);
				currentObject_->SetTint(newTint);
			}

			// Show the diffuse coefficient of the object
			glm::vec3 diffuse = currentObject_->GetDiffuse();
			colors[0] = diffuse.r;
			colors[1] = diffuse.g;
			colors[2] = diffuse.b;
			
			ImGui::Text("Diffuse:");
			if (ImGui::ColorEdit3("Diffuse Color", colors))
			{
				glm::vec3 newD(colors[0], colors[1], colors[2]);
				currentObject_->SetDiffuse(newD);
			}

			// Show the specular coefficient of the object
			glm::vec3 specular;
			float specularExp;
			modified = false;
			currentObject_->GetSpecular(&specular, &specularExp);
			colors[0] = specular.r;
			colors[1] = specular.g;
			colors[2] = specular.b;
			ImGui::Text("Specular");
			if (ImGui::ColorEdit3("Specular Color", colors))
				modified = true;
			if (ImGui::DragFloat("Exponent", &specularExp, 0.1f, INT_MIN, INT_MAX, "%.02f", 1.0f))
				modified = true;

			if (modified)
			{
				glm::vec3 newS(colors[0], colors[1], colors[2]);
				currentObject_->SetSpecular(newS, specularExp);
			}

			// End the window
			ImGui::End();
		}
	}
}

ImGUISystem::~ImGUISystem()
{

}
