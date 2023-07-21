#include "FastCrossbow.h"

using namespace std;
FastCrossbow::FastCrossbow() : IModule(0, Category::COMBAT, "Shoot crossbow arrow for an instant") {
	registerIntSetting("StartSlot", &start, start, 1, 9);
	registerIntSetting("EndSlot", &slot, slot, 1, 9);
}

const char* FastCrossbow::getModuleName() {
	return ("FastCrossbow");
}
const char* FastCrossbow::getRawModuleName() {
	return ("FastCrossbow");
}
void FastCrossbow::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto selectedItem = player->getSelectedItem();

	if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		auto prevSlot = supplies->selectedHotbarSlot;

		for (int n = start - 1; n < slot; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 575) {  // select crystal
					supplies->selectedHotbarSlot = n;
					gm->useItem(*stack);

					// return;
				}
			}
		}
		supplies->selectedHotbarSlot = prevSlot;
	}
	// setEnabled(false);
}
