#include"Offhand.h"
#include "../../../SDK/CItem.h"
#include "../ModuleManager.h"

using namespace std;
Offhand::Offhand() : IModule(0, Category::PLAYER, "Automatically puts totems or shield into your offhand") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry(EnumEntry("Totem", 0));
	mode.addEntry(EnumEntry("Shield", 1));
}

const char* Offhand::getModuleName() {
	return ("Offhand");
}

void Offhand::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void Offhand::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (mode.getSelectedValue() == 0) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		C_ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();
		if (current->item == NULL) {
			for (int i = 1; i < 36; i++) {
				C_ItemStack* totem = inv->getItemStack(i);
				if (totem->item != NULL && (*totem->item)->itemId == 568) {
					g_Data.getLocalPlayer()->setOffhandSlot(totem);
					inv->removeItem(i, 1);
					break;
				}
			}
		}
	} else {
		C_PlayerInventoryProxy* suppliesg = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* invg = suppliesg->inventory;
		C_ItemStack* currentg = g_Data.getLocalPlayer()->getEquippedTotem();
		if (currentg->item == NULL) {
			for (int cc = 1; cc < 36; cc++) {
				C_ItemStack* totemcc = invg->getItemStack(cc);
				if (totemcc->item != NULL && (*totemcc->item)->itemId == 355) {
					g_Data.getLocalPlayer()->setOffhandSlot(totemcc);
					invg->removeItem(cc, 1);
					break;
				}
			}
		}
	}
}

void Offhand::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}