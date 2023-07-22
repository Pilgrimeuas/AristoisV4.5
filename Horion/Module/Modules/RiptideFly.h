#pragma once
#include"Module.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
class RiptideFly : public IModule {
private:
	float speed = 0.325f;
	float upanddown = 0.6f;
	float glideMod = -0.00f;
	float glideModEffective = 0;
	

	

public:
	bool trident;
	bool rotation;
	vec2_t angle;
	bool Remp = false;
	RiptideFly();
	~RiptideFly();

	// Inherited via IModule
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	virtual void onMove(C_MoveInputHandler* input);
};