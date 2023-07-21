#pragma once
#include "Module.h"
class MidClickPearl : public IModule {
private:
	bool hasClicked = false;
	bool spoof = true;
	

public:
	MidClickPearl();
	~MidClickPearl();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
