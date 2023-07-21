#pragma once
#include "Module.h"

class Reach : public IModule {
private:

	float originalReach = 0;
	float* reachPtr = nullptr;
	unsigned long oldProtect = 0;

public:
	float reachValue = 3;
	Reach();
	~Reach();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
};
