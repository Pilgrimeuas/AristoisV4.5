#pragma once
#include "Module.h"
class BigTest : public IModule {
private:
	int delay = 0;
	int delay2 = 0;
	int delay3 = 0;

public:
	float float1 = 0;
	float float2 = 0;
	float float3 = 0;
	int int1 = 0;
	int int2 = 0;
	int int3 = 0;
	bool bool1 = false;
	bool bool2 = false;
	bool bool3 = false;
	SettingEnum enum1;
	SettingEnum enum2;

	BigTest();
	~BigTest();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual bool isFlashMode() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* p) override;
	virtual void onMove(C_MoveInputHandler* hand) override;
	virtual void onDisable() override;
	void onLevelRender() override;
};
