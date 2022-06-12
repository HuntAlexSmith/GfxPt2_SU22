#pragma once
//*****************************************************************************
//	File:   Camera.h
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: Class to hold the data of a 3D camera
//*****************************************************************************

#include "GfxMath.h"

//*****************************************************************************
//  Description:
//		Camera class, which handles all the data required for a 3D camera, as
//		well as any modification functions for moving the camera
//*****************************************************************************
class Camera {
public:

	Camera(glm::vec4 eye, glm::vec4 lookAt, float fov, float aspect, float near, float far);

	void ForwardMove(float amount);
	void SideMove(float amount);

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);

	void SetAspect(float aspect);

	float GetFOV();

	void Zoom(float zoom);

	glm::vec4 GetLookAt();
	glm::vec4 GetEyePoint();

	glm::mat4 GetCamToWorldMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetPerspMatrix();

	~Camera();

private:

	// Private function for setting dirty flag
	void SetDirtyFlags(bool val);

	// Matrices from camera
	glm::mat4 camToWorldMat_;
	glm::mat4 viewMat_;
	glm::mat4 perspMat_;

	// Dirty flags for matrices
	bool camToWorldIsDirty_;
	bool viewIsDirty_;
	bool perspIsDirty_;

	// Eye point of the camera
	glm::vec4 eyePoint_;
	
	// Current fov of the camera
	float fov_;

	// Look At Vector for the Camera
	glm::vec4 lookAtVec_;

	// Orientation vectors for the camera
	glm::vec4 rightVec_;
	glm::vec4 upVec_;
	glm::vec4 backVec_;

	// Dimensions of the Camera
	float aspectRatio_;
	float viewportWidth_;
	float viewportHeight_;

	// Distances
	float nearDist_;
	float viewportDist_;
	float farDist_;
};