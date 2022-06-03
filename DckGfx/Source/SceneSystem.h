#pragma once
//*****************************************************************************
//	File:   SceneSystem.h
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Scene System that will manage the different Scenes that the
//		renderer will render
//*****************************************************************************

#include "System.h"
#include "SceneList.h"

typedef void (*LoadFunc)();
typedef void (*InitFunc)();
typedef void (*UpdateFunc)(float);
typedef void (*ShutdownFunc)();
typedef void (*UnloadFunc)();

class SceneSystem : public System {
public:
	struct Scene {
		LoadFunc load;
		InitFunc init;
		UpdateFunc update;
		ShutdownFunc shutdown;
		UnloadFunc unload;
		Scene() :
			load(nullptr),
			init(nullptr),
			update(nullptr),
			shutdown(nullptr),
			unload(nullptr)
		{}
		Scene(LoadFunc l, InitFunc i, UpdateFunc u, ShutdownFunc s, UnloadFunc ul) :
			load(l),
			init(i),
			update(u),
			shutdown(s),
			unload(ul)
		{}
	};

	SceneSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void SetNextScene(SceneID nextScene);

	~SceneSystem();

private:

	bool SceneIsValid(SceneID scene);


	Scene scenes_[SceneID::SceneCount];

	SceneID prevScene_;
	SceneID currScene_;
	SceneID nextScene_;

};