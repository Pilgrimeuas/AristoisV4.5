#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Offhand : public IModule {
public:
	SettingEnum mode = this;

	virtual void onDisable();
	virtual void onEnable();
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	Offhand();
};