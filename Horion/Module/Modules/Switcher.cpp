#include "Switcher.h"

#include "../ModuleManager.h"
#include "Module.h"

Switcher::Switcher() : IModule(0, Category::COMBAT, "Switches between hotbar slots") {
	// registerBoolSetting("DoubleSwitch", &ss, ss);
	// registerBoolSetting("AutoSwitch", &autos, autos);
	registerIntSetting("Slots", &ternary, 1, 0, 8);
	registerIntSetting("Delay", &delay, delay, 1, 10);
}

const char* Switcher::getModuleName() {
	return ("Switcher");
}

void Switcher::onEnable() {
}

void Switcher::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	delay1++;
	if (delay1 >= delay) {
		if (autos && !ownage) {
			ternary1++;
			if (ternary1 >= ternary + 0) {
				ternary1 = 0;
			}
			supplies->selectedHotbarSlot = ternary1;
		}
		delay1 = 0;
	}
}