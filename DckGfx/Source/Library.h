#pragma once
//*****************************************************************************
//	File:   Library.h
//  Author: Hunter Smith
//  Date:   06/03/2022
//  Description: Base class for data libraries (meshes, textures, shaders).
//		Going to be templated???
//*****************************************************************************

#include <string>
#include <map>

template <typename T>
class Library {
public:

	Library() {}

	virtual void Initialize() = 0;

	virtual void AddObject(std::string name, T obj) = 0;

	virtual T GetObject(std::string objName) = 0;

	virtual void Shutdown() = 0;

	~Library() {}

private:
};