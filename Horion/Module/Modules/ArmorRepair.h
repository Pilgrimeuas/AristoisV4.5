#pragma once
#include "Module.h"
class ArmorRepair : public IModule {
private:
	bool hasClicked = false;
	bool spoof = false;
	bool MidClick = false;

public:
	int fakeSlot = -1;
	ArmorRepair();
	~ArmorRepair();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
