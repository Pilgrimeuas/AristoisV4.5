#pragma once
#include"Module.h"
#include "../ModuleManager.h"

class AutoObsidian : public IModule {
private:
	int range = 5;
	bool autopickaxe = true;

public:
	AutoObsidian();
	~AutoObsidian();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};