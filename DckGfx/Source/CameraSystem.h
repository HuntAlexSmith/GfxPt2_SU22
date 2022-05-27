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
#include "Mesh.h"
#include <map>

class CameraSystem : public System {
public:

	CameraSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	Camera* GetActiveCamera();

	~CameraSystem();

private:

	// Orientation mesh, which is used to understand 
	Mesh* orientationMesh_;
	
	// A map of all the Cameras being managed by the system
	std::map<unsigned int, Camera*> cameras_;

	// Pointer to the current active camera
	Camera* activeCam_;

};