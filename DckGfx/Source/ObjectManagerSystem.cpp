//*****************************************************************************
//	File:   ObjectManagerSystem.cpp
//  Author: Hunter Smith
//  Date:   06/14/2022
//  Description: System for managing all objects that are currently active in
//		the scene
//*****************************************************************************

#include "ObjectManagerSystem.h"
#include <queue>

ObjectManagerSystem::ObjectManagerSystem() : System(ObjectManagerSys)
{

}

void ObjectManagerSystem::Initialize()
{

}

void ObjectManagerSystem::Update(float dt)
{
	// Keep track of all objects removed from the list
	std::queue<std::vector<RenderObject*>::iterator> itrQueue;
	
	// Go through the list and delete any objects marked for deletion
	unsigned int size = objects_.size();
	for (int i = 0; i < size; ++i)
	{
		RenderObject* currObj = objects_[i];
		if (currObj)
		{
			currObj->Draw();
			if (currObj->IsDestroyed())
			{
				itrQueue.push(objects_.begin() + i);
				delete currObj;
				objects_[i] = nullptr;
			}
		}
	}

	// Now remove all spaces that had objects in them
	while (!itrQueue.empty())
	{
		auto currItr = itrQueue.front();
		itrQueue.pop();
		objects_.erase(currItr);
	}
}

void ObjectManagerSystem::Shutdown()
{
	for (RenderObject* object : objects_)
	{
		if (object)
			delete object;
	}
	objects_.clear();
}

void ObjectManagerSystem::AddObject(RenderObject* obj)
{
	if (obj)
		objects_.push_back(obj);
}

RenderObject* ObjectManagerSystem::GetObject(std::string name)
{
	for (RenderObject* object : objects_)
	{
		if (object->GetName() == name)
			return object;
	}
	return nullptr;
}

void ObjectManagerSystem::ClearManager()
{
	Shutdown();
}

RenderObject** ObjectManagerSystem::GetAllObjects()
{
	if (objects_.size() > 0)
		return &(objects_[0]);
}

int ObjectManagerSystem::GetCount()
{
	return objects_.size();
}

ObjectManagerSystem::~ObjectManagerSystem()
{

}