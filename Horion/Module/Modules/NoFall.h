#pragma once

#include "../../DrawUtils.h"
#include "Module.h"

class NoFall : public IModule {
private:
	float distanceToGround();

public:
	SettingEnum mode;

	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend) ;
	virtual void onTick(C_GameMode* gm) override;
};