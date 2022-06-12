//*****************************************************************************
//	File:   CameraSystem.cpp
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: Camera System which will manage the different cameras in a
//		scene
//*****************************************************************************

#include "Engine.h"
#include "CameraSystem.h"
#include "InputSystem.h"

static const float moveAmount = 10.0f;
static const float rotAngle = 45.0f;
static const float mouseSen = 1.0f;

// Orientation mesh colors
static const glm::vec3 red(1, 0, 0);
static const glm::vec3 green(0, 1, 0);
static const glm::vec3 blue(0, 0, 1);

// Orientation mesh vertices
static const glm::vec4 center = GfxMath::Point(0, 0, 0);
static const glm::vec4 xDir = GfxMath::Point(1, 0, 0);
static const glm::vec4 yDir = GfxMath::Point(0, 1, 0);
static const glm::vec4 zDir = GfxMath::Point(0, 0, 1);

CameraSystem::CameraSystem() : System(SysType::CameraSys),
	prevMousePos_(GfxMath::Point(0, 0)),
	currMousePos_(GfxMath::Point(0, 0)),
	oMesh_(nullptr),
	cameras_(),
	activeCam_(nullptr)
{
}

void CameraSystem::Initialize()
{
	// Create an initial camera and add it to the camera map
	Camera* startCam = new Camera(GfxMath::Point(0, 0, -10), GfxMath::Vector(0, 0, 1), 90.0f, 16.0f / 9.0f, 1.0f, 100.0f);
	std::pair<unsigned int, Camera*> camToAdd(1, startCam);
	cameras_.insert(camToAdd);

	// Set the active camera to this new camera
	activeCam_ = startCam;
}

void CameraSystem::Update(float dt)
{
	if (!oMesh_)
	{
		oMesh_ = MeshLibraryGet("CamOMesh");
	}

	// Handle any input for the current camera here
	InputSystem* inputSys = dynamic_cast<InputSystem*>(GetParent()->GetSystem(SysType::InputSys));
	if (inputSys)
	{
		// Basic movement of the camera's position
		if (inputSys->KeyIsDown(SDLK_d))
			activeCam_->SideMove(moveAmount*dt);
		else if (inputSys->KeyIsDown(SDLK_a))
			activeCam_->SideMove(-moveAmount*dt);

		if (inputSys->KeyIsDown(SDLK_w))
			activeCam_->ForwardMove(moveAmount*dt);
		else if (inputSys->KeyIsDown(SDLK_s))
			activeCam_->ForwardMove(-moveAmount*dt);

		// Orientation of the camera
		if (inputSys->KeyIsDown(SDLK_LEFT))
			activeCam_->Roll(rotAngle * dt);
		else if (inputSys->KeyIsDown(SDLK_RIGHT))
			activeCam_->Roll(-rotAngle * dt);

		// Zoom of the camera
		if (inputSys->KeyIsDown(SDLK_UP))
			activeCam_->Zoom(rotAngle * dt);
		else if (inputSys->KeyIsDown(SDLK_DOWN))
			activeCam_->Zoom(rotAngle * -dt);

		// Mouse input for orientation of the camera
		if (inputSys->RMIsTriggered())
			inputSys->GetMouseScreenPos(&currMousePos_.x, &currMousePos_.y);
		else if (inputSys->RMIsDown())
		{
			// Update mouse position stuff
			prevMousePos_ = currMousePos_;
			inputSys->GetMouseScreenPos(&currMousePos_.x, &currMousePos_.y);

			glm::vec4 mouseDist = GfxMath::Vector(currMousePos_.x - prevMousePos_.x, currMousePos_.y - prevMousePos_.y);

			// Get the X rotation and rotate the camera
			if (currMousePos_.x != prevMousePos_.x)
			{
				activeCam_->Yaw(-mouseDist.x*mouseSen);
			}

			// Get the Y rotation and rotate the camera
			if (currMousePos_.y != prevMousePos_.y)
			{
				activeCam_->Pitch(-mouseDist.y*mouseSen);
			}
		}
	}

	// Get the translation mat
	glm::vec4 frontVec = activeCam_->GetLookAt();
	glm::vec4 eye = activeCam_->GetEyePoint();
	frontVec *= 15;
	glm::vec4 orientPos = eye + frontVec;
	glm::mat4 translate = GfxMath::Translate(orientPos) * GfxMath::Scale3D(activeCam_->GetFOV() / 90.0f);

	glm::mat4 translation = translate;

	// Render the mesh
	GetParent()->DebugRender(oMesh_, RenderType::Points, translation);
	GetParent()->DebugRender(oMesh_, RenderType::Lines, translation);
}

void CameraSystem::Shutdown()
{
	// Delete all the camera's in the system here
	for (auto cam : cameras_)
	{
		if (cam.second)
			delete cam.second;
	}
	cameras_.clear();
}

void CameraSystem::UpdateAspects(float aspect)
{
	for (auto cam : cameras_)
	{
		if (cam.second)
			cam.second->SetAspect(aspect);
	}
}

Camera* CameraSystem::GetActiveCamera()
{
	return activeCam_;
}

CameraSystem::~CameraSystem()
{

}
