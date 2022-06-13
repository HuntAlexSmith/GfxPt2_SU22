#pragma once
//*****************************************************************************
//	File:   System.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: Base System class for any and all systems that will be part
//		of the rendering engine
//*****************************************************************************

// Forward declare engine so System knows what it is
class Engine;

class System {
public:

	enum SysType {
		WindowSys,
		InputSys,
		CameraSys,
		GraphicsSys,
		LightingSys,
		RenderSys,
		ImGUISys,
		SceneSys,
		SystemCount
	};

	System(SysType type) : type_(type), parent_(nullptr) {}

	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Shutdown() = 0;

	SysType GetType() { return type_; }

	Engine* GetParent() { return parent_; }
	void SetParent(Engine* engine) { parent_ = engine; }

	~System() {}

private:

	SysType type_;
	Engine* parent_;

};