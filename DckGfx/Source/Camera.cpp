//*****************************************************************************
//	File:   Camera.cpp
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: Class to hold the data of a 3D camera
//*****************************************************************************

#include "Camera.h"

static const glm::vec4 relativeUp = GfxMath::Vector(0, 1, 0);

Camera::Camera(glm::vec4 eye, glm::vec4 lookAt, float fov, float aspect, float near, float far) :
	worldToCamMat_(),
	viewMat_(),
	perspMat_(),
	worldToCamIsDirty_(true),
	viewIsDirty_(true),
	perspIsDirty_(true),
	eyePoint_(eye),
	lookAtVec_(lookAt),
	rightVec_(),
	upVec_(),
	backVec_(),
	aspectRatio_(aspect),
	viewportWidth_(),
	viewportHeight_(),
	nearDist_(near),
	viewportDist_(),
	farDist_(far),
	yaw_(0.0f),
	pitch_(0.0f),
	roll_(0.0f)
{
	// Calculate back vector
	backVec_ = -lookAt / glm::length(lookAt);

	// Calculate right vector
	glm::vec4 lookCrossRel = GfxMath::CrossProduct(lookAt, relativeUp);
	rightVec_ = lookCrossRel / glm::length(lookCrossRel);

	// Calculate up vector
	upVec_ = GfxMath::CrossProduct(backVec_, rightVec_);

	// Calculate the viewport distance (half-way between near and far)
	viewportDist_ = nearDist_ + (farDist_ - nearDist_) / 2.0f;

	// Calculate width and height of the camera
	viewportWidth_ = 2 * viewportDist_ * tanf(fov / 2.0f);
	viewportHeight_ = viewportWidth_ / aspectRatio_;
}

void Camera::XMove(float amount)
{
	eyePoint_.x += amount;
	SetDirtyFlags(true);
}

void Camera::YMove(float amount)
{
	eyePoint_.y += amount;
	SetDirtyFlags(true);
}

void Camera::ZMove(float amount)
{
	eyePoint_.z += amount;
	SetDirtyFlags(true);
}

void Camera::Yaw(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(upVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	yaw_ += angle;

	SetDirtyFlags(true);
}

void Camera::Pitch(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(rightVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	pitch_ += angle;

	SetDirtyFlags(true);
}

void Camera::Roll(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(backVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	roll_ += angle;

	SetDirtyFlags(true);
}

float Camera::GetYaw()
{
	return yaw_;
}

float Camera::GetPitch()
{
	return pitch_;
}

float Camera::GetRoll()
{
	return roll_;
}

glm::vec4 Camera::GetLookAt()
{
	return -backVec_;
}

glm::vec4 Camera::GetEyePoint()
{
	return eyePoint_;
}

void Camera::SetDirtyFlags(bool val)
{
	worldToCamIsDirty_ = val;
	viewIsDirty_ = val;
	perspIsDirty_ = val;
}

glm::mat4 Camera::GetWorldToCamMatrix()
{
	if (worldToCamIsDirty_)
	{
		worldToCamMat_ = GfxMath::Affine(rightVec_, upVec_, backVec_, eyePoint_);
		worldToCamIsDirty_ = false;
	}
	return worldToCamMat_;
}

glm::mat4 Camera::GetViewMatrix()
{
	if (viewIsDirty_)
	{
		if (worldToCamIsDirty_)
			GetWorldToCamMatrix();

		viewMat_ = GfxMath::AffineInverse(worldToCamMat_);
		viewIsDirty_ = false;
	}
	return viewMat_;
}

glm::mat4 Camera::GetPerspMatrix()
{
	if (perspIsDirty_)
	{
		perspMat_ = glm::mat4(
			2 * viewportDist_ / viewportWidth_, 0, 0, 0,
			0, 2 * viewportDist_ / viewportHeight_, 0, 0,
			0, 0, (nearDist_ + farDist_) / (nearDist_ - farDist_), -1,
			0, 0, (2 * nearDist_ * farDist_) / (nearDist_ - farDist_), 0
		);
		perspIsDirty_ = false;
	}
	return perspMat_;
}

Camera::~Camera()
{

}