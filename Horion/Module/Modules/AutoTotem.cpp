#include "AutoTotem.h"

#include "../../../SDK/CItem.h"
#include "../ModuleManager.h"
#include "Module.h"
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Auto Totem WTF") {
}

AutoTotem::~AutoTotem() {
}

const char* AutoTotem::getModuleName() {
	return ("AutoTotem");
}

void AutoTotem::onEnable() {
}

void AutoTotem::onTick(C_GameMode* gm) {
	if (!totem)
		return;

	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();

	bool run = false;

	if (current->item == NULL)
		run = true;

	else if ((*current->item)->itemId != (this->totem ? 568 : 355))
		run = true;

	if (run) {
		for (int i = 0; i < 36; i++) {
			C_ItemStack* totem = inv->getItemStack(i);
			if (totem->item != NULL && (*totem->item)->itemId == (this->totem ? 568 : 355)) {
				C_InventoryAction first(i, totem, nullptr);
				C_InventoryAction second(37, nullptr, totem);
				g_Data.getLocalPlayer()->setOffhandSlot(totem);
				manager->addInventoryAction(first);
				manager->addInventoryAction(second);
				break;
			}
		}
	}
}