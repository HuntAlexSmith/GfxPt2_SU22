#pragma once
//*****************************************************************************
//	File:   ObjectManagerSystem.h
//  Author: Hunter Smith
//  Date:   06/14/2022
//  Description: System for managing all objects that are currently active in
//		the scene
//*****************************************************************************

#include "System.h"
#include "RenderObject.h"
#include <vector>

class ObjectManagerSystem : public System {
public:

	ObjectManagerSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void AddObject(RenderObject* obj);
	RenderObject* GetObject(std::string name);
	void ClearManager();

	RenderObject** GetAllObjects();
	int GetCount();

	~ObjectManagerSystem();

private:

	std::vector<RenderObject*> objects_;

};