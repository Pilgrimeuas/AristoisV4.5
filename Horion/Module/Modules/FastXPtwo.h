#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class FastXPtwo : public IModule {
public:
	int delay = 0;
	int Odel = 0;
	bool okPacketSent = false;
	int multiplier = 0;
	FastXPtwo() : IModule(0x0, Category::PLAYER, "throw exp bottles more very fast") {
		registerIntSetting("SentPacket", &multiplier, multiplier, 1, 30);
	};
	~FastXPtwo(){};

	void onTick(C_GameMode* gm) {
		C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
		if (GameData::isRightClickDown() && stack->item != nullptr && (*stack->item)->itemId != 0 && (stack->getItem()->itemId == 508 || stack->getItem()->itemId == 374 || stack->getItem()->itemId == 300))
			okPacketSent = true;
		else
			okPacketSent = false;
	}

	virtual const char* getModuleName() override {
		return "MultiUse";
	}
};
