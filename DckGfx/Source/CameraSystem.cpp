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

static const float moveAmount = 0.1f;
static const float rotAngle = 1.0f;

// Orientation mesh colors
static const glm::vec3 red(1, 0, 0);
static const glm::vec3 green(0, 1, 0);
static const glm::vec3 blue(0, 0, 1);

// Orientation mesh vertices
static const glm::vec4 center = GfxMath::Point(0, 0, 0);
static const glm::vec4 xDir = GfxMath::Point(1, 0, 0);
static const glm::vec4 yDir = GfxMath::Point(0, 1, 0);
static const glm::vec4 zDir = GfxMath::Point(0, 0, 1);

static const glm::vec4 xVec = GfxMath::Vector(1, 0, 0);
static const glm::vec4 yVec = GfxMath::Vector(0, 1, 0);
static const glm::vec4 zVec = GfxMath::Vector(0, 0, 1);

CameraSystem::CameraSystem() : System(SysType::CameraSys), orientationMesh_(nullptr), cameras_(), activeCam_(nullptr)
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
	if (!orientationMesh_)
	{
		// Create the orientation mesh
		orientationMesh_ = new Mesh();

		orientationMesh_->AddVertex(center, red);
		orientationMesh_->AddVertex(center, green);
		orientationMesh_->AddVertex(center, blue);
		orientationMesh_->AddVertex(xDir, red);
		orientationMesh_->AddVertex(yDir, green);
		orientationMesh_->AddVertex(zDir, blue);

		orientationMesh_->AddEdge(0, 3);
		orientationMesh_->AddEdge(1, 4);
		orientationMesh_->AddEdge(2, 5);
	}

	// Handle any input for the current camera here
	InputSystem* inputSys = dynamic_cast<InputSystem*>(GetParent()->GetSystem(SysType::InputSys));
	if (inputSys)
	{
		if (inputSys->KeyIsDown(SDLK_d))
			activeCam_->XMove(moveAmount);
		else if (inputSys->KeyIsDown(SDLK_a))
			activeCam_->XMove(-moveAmount);

		if (inputSys->KeyIsDown(SDLK_w))
			activeCam_->ZMove(moveAmount);
		else if (inputSys->KeyIsDown(SDLK_s))
			activeCam_->ZMove(-moveAmount);

		if (inputSys->KeyIsDown(SDLK_UP))
			activeCam_->Pitch(rotAngle);
		else if (inputSys->KeyIsDown(SDLK_DOWN))
			activeCam_->Pitch(-rotAngle);

		if (inputSys->KeyIsDown(SDLK_LEFT))
			activeCam_->Yaw(rotAngle);
		else if (inputSys->KeyIsDown(SDLK_RIGHT))
			activeCam_->Yaw(-rotAngle);
	}

	// Get rotations of each direction
	glm::mat4 yawRot = GfxMath::Rotate3D(yVec, activeCam_->GetYaw());
	glm::mat4 pitchRot = GfxMath::Rotate3D(xVec, activeCam_->GetPitch());
	glm::mat4 rollRot = GfxMath::Rotate3D(zVec, activeCam_->GetRoll());

	// Get the translation mat
	glm::vec4 frontVec = activeCam_->GetLookAt();
	glm::vec4 eye = activeCam_->GetEyePoint();
	frontVec *= 8;
	glm::vec4 orientPos = eye + frontVec;
	glm::mat4 translate = GfxMath::Translate(orientPos);

	glm::mat4 translation = translate * pitchRot * yawRot * rollRot;

	// Render the mesh
	GetParent()->Render(orientationMesh_, RenderType::Lines, translation);
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

	if (orientationMesh_)
		delete orientationMesh_;
}

Camera* CameraSystem::GetActiveCamera()
{
	return activeCam_;
}

CameraSystem::~CameraSystem()
{

}
