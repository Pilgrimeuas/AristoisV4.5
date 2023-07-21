#pragma once
#include "Module.h"

class Switcher : public IModule {
private:
	int ternary = false;
	int ternary1 = false;
	bool autos = true;
	bool ss = false;
	bool swap = false;
	bool ownage = false;
	bool ternaryO = false;
	bool toggler = false;
	int delay = false;
	int delay1 = false;

public:
	Switcher();

	// virtual void onAttack(C_Entity* attackedEnt);
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName();
	virtual void onEnable();
};