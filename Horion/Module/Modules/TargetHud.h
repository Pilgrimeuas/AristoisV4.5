#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TargetHud : public IModule {
private:
	int Odelay = 0;
	float height = 1.f;
	bool autoweapon = false;

public:
	bool rot = false;
	// bool holdMode = false;
	int targethud = 0;
	int delay = 0;
	bool cock = false;
	bool packetcock = false;
	bool silent = true;
	bool isMulti = true;
	bool distanceCheck = false;
	bool isMobAura = false;
	bool hurttime = false;
	int range = 8;
	int x = 15;
	int y = 15;

	TargetHud();
	~TargetHud();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	// virtual void onSendPacket(C_Packet* packet) override;
};