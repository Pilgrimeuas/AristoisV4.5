#pragma once
#include "Module.h"
class Timer : public IModule {
private:
	float timer = 25;

public:
	Timer();
	~Timer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
};
