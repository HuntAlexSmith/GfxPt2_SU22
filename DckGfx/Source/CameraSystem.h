#pragma once
//*****************************************************************************
//	File:   CameraSystem.h
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: Camera System which will manage the different cameras in a
//		scene
//*****************************************************************************

#include "System.h"
#include "Camera.h"
#include "MeshLib.h"
#include <map>

class CameraSystem : public System {
public:

	CameraSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void UpdateAspects(float aspect);

	Camera* GetActiveCamera();

	~CameraSystem();

private:

	// Previous and current mouse positions
	glm::ivec4 prevMousePos_;
	glm::ivec4 currMousePos_;

	// Orientation mesh, which is used to understand 
	DckMesh* oMesh_;
	
	// A map of all the Cameras being managed by the system
	std::map<unsigned int, Camera*> cameras_;

	// Pointer to the current active camera
	Camera* activeCam_;

};