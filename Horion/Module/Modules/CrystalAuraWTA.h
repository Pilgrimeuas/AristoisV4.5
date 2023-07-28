#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class CrystalAuraWTA : public IModule {
private:
	int delay = 0;
	bool autoplace = true;
	bool crystalCheck = true;
	bool yLock = false;
	bool isClick = false;
	//	bool doMultiple = true;
	bool silent = false;
	int slotCA = 0;
	bool renderCA = true;
	bool oldrdca = false;

public:
	CrystalAuraWTA();
	~CrystalAuraWTA();

	// Inherited via IModule
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onEnable();
	virtual void onDisable();
	int range = 7;
	// bool BadMan = true;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	vec3_t espPosUpper;
	vec3_t espPosLower;
};
#pragma once
