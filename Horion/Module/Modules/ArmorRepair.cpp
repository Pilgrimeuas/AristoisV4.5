#include "ArmorRepair.h"

ArmorRepair::ArmorRepair() : IModule(0, Category::PLAYER, "Spoofing Gapple Or XP || By Sadami") {
	registerBoolSetting("Spoof", &spoof, spoof);
	registerBoolSetting("MidClick", &MidClick, MidClick);
}

ArmorRepair::~ArmorRepair() {
}

const char* ArmorRepair::getModuleName() {
	return "PacketAction";
}
bool spswap52(int Itemid) {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId != 0) {
				if (stack->getItem()->itemId == Itemid) {
					C_MobEquipmentPacket a(id, *stack, n, n);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
					return true;
				}
			}
		}
	}
	// gm->baseUseItem(*n);
	return false;
}
bool stopsp52() {
	__int64 id = -1;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	// C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	// C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 259) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
}
void getXPBottle() {  // get Piston 29
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 508) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void ArmorRepair::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (MidClick) {
		if (GameData::isWheelDown()) {
			if (spoof) {
				spswap52(508);
				C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
				gm->baseUseItem(*stack);
				// gm->useItem(*stack);
				stopsp52();
			}
			if (!spoof) {
				getXPBottle();
				// auto supplies = g_Data.getLocalPlayer()->getSupplies();
				C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
				gm->baseUseItem(*stack);
				// gm->useItem(*stack);
			}
			hasClicked = true;
		} else if (!GameData::isWheelDown()) {
			hasClicked = false;
		}
	}
	if (!MidClick) {
		if (spoof) {
			spswap52(508);
			C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
			gm->baseUseItem(*stack);
			// gm->useItem(*stack);
			stopsp52();
		}
		if (!spoof) {
			getXPBottle();
			// auto supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
			gm->baseUseItem(*stack);
			// gm->useItem(*stack);
		}
	}
}
