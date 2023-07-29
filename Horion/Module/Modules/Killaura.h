#pragma once

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class Killaura : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	SettingEnum autoweapon;
	void findWeapon();
	bool silent = true;
	int slot;

public:
	SettingEnum priority;
	//bool rotations = false;
	bool targetListEmpty = false;
	bool swingKA = true;
	int slotKA = 0;

	vec2_t angle;
	bool strafe = false;
	float timer = 20;
	bool mobs = false;
	bool isMobAura = false;
	bool isSingleAura = false;
    bool isMulitAura = false;

	bool timer1 = false;
	bool hurttime = false;
	float range = 6;
	SettingEnum rotations = this;
	SettingEnum mode = this;
	vec2_t tro;
	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPlayerTick(C_Player* plr);
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet);
};