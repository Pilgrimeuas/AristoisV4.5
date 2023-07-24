#pragma once
#include"Module.h"

class Timer2 : public IModule {
private:
	int timer = 50;

public:
	Timer2();
	~Timer2();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
};
