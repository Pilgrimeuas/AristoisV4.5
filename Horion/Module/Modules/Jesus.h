#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Jesus : public IModule {
private:
	bool tryJesus(vec3_t blockBelow);
	bool viewBobbingEffective = false;
	bool velocityEffective = false;
	bool wasInWater = false;
	float height = 0.5;

public:
	bool foundWater = false;

	std::string name = "Jesus";
	SettingEnum mode = this;

	virtual void onMove(C_MoveInputHandler* input);
	virtual const char* getModuleName();
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	Jesus();
};


