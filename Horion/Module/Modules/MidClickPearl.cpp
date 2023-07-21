#include "MidClickPearl.h"
MidClickPearl::MidClickPearl() : IModule(0, Category::PLAYER, "By Sadami") {
	registerBoolSetting("Spoof", &spoof, spoof);
}

MidClickPearl::~MidClickPearl() {
}

const char* MidClickPearl::getModuleName() {
	return "FastPearl";
}
bool spswap114514(int Itemid) {
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
	//gm->baseUseItem(*n);
	return false;
}
bool stopsp1145142() {
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
void getPearl() {  // get Piston 29
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 422) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void MidClickPearl::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;


	if (!hasClicked) {
		if (spoof) {
			spswap114514(422);
			C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
			gm->baseUseItem(*stack);
			//gm->useItem(*stack);
			stopsp1145142();
		}
		if (!spoof) {
			getPearl();
			//auto supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
			gm->baseUseItem(*stack);
			//gm->useItem(*stack);
		}
		hasClicked = true;
	} else if (hasClicked) {
		this->setEnabled(false);
		hasClicked = false;
	}
}
