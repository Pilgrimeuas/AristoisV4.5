#pragma once

#include "Module.h"

class Disabler : public IModule {
public:
	bool hive = true;
	bool elytra = false;
	bool air = false;
	float speed = 1.f;
	float glideMod = 0.f;
	float glideModEffective = 0;
	int counter = 1;
	Disabler();
	virtual const char* getModuleName();

	void onTick(C_GameMode* gm);
	
	virtual void onPlayerTick(C_Player* plr) override;
	//virtual void onSendPacket(C_Packet* packet) override;
	
};
#pragma once
