#pragma once

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class Killaura : public IModule {
private:
	bool isMulti = true;
	int delay = 0;
	int Odelay = 0;
	SettingEnum autoweapon;
	void findWeapon();
	bool silent = true;
	int slot;

public:
	SettingEnum priority;
	bool rotations = false;
	bool targetListEmpty = false;
	vec2_t angle;
	bool strafe = false;
	float timer = 20;
	bool isMobAura = false;
	bool timer1 = false;
	bool hurttime = false;
	float range = 6;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet);
};