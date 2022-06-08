//*****************************************************************************
//	File:   RenderObject.cpp
//  Author: Hunter Smith
//  Date:   06/07/2022
//  Description: Object that will handle an objects mesh, color, diffuse,
//		specular, and position data. Makes for easier rendering
//*****************************************************************************

#include "RenderObject.h"
#include "DckGfxEngine.h"

RenderObject::RenderObject() :
	mesh_(nullptr),
	pos_(0),
	scale_(1),
	rotVec_(GfxMath::Vector(0, 1, 0)),
	rotAngle_(0.0f),
	tint_(0),
	diffuse_(0),
	specular_(0),
	specularExp_(0.0f),
	isDirty_(true),
	modelMat_(1)
{
}

void RenderObject::SetMesh(DckMesh* mesh)
{
	mesh_ = mesh;
	isDirty_ = true;
}

void RenderObject::SetPosition(glm::vec4 pos)
{
	pos_ = pos;
	isDirty_ = true;
}

void RenderObject::SetScale(glm::vec3 scale)
{
	scale_ = scale;
	isDirty_ = true;
}

void RenderObject::SetRotation(glm::vec4 vec, float angle)
{
	rotVec_ = vec;
	rotAngle_ = angle;
	isDirty_ = true;
}

void RenderObject::SetTint(glm::vec3 tint)
{
	tint_ = tint;
	isDirty_ = true;
}

void RenderObject::SetDiffuse(glm::vec3 coeff)
{
	diffuse_ = coeff;
	isDirty_ = true;
}

void RenderObject::SetSpecular(glm::vec3 coeff, float exp)
{
	specular_ = coeff;
	specularExp_ = exp;
	isDirty_ = true;
}

void RenderObject::GetRotation(glm::vec4* vec, float* angle)
{
	*vec = rotVec_;
	*angle = rotAngle_;
}

void RenderObject::Draw(RenderType type)
{
	if (isDirty_)
	{
		modelMat_ = GfxMath::Translate(pos_) * GfxMath::Rotate3D(rotVec_, rotAngle_) * GfxMath::Scale(scale_.x, scale_.y, scale_.z);
		isDirty_ = false;
	}

	DckERender(mesh_, type, modelMat_, tint_, diffuse_, specular_, specularExp_);
}

RenderObject::~RenderObject()
{
}
