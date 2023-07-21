#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class AutoCity : public IModule {
private:
	bool hasClicked = false;
	bool silent = false;
	bool isMulti = false;
	bool swaped = false;

public:
	int face = 0;
	vec3_ti pos;
	int odelays = 0;
	int delay = 0;
	float range = 10.f;
	AutoCity();
	~AutoCity();
	virtual const char* getModuleName() override;
	virtual void findWeapon();
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};