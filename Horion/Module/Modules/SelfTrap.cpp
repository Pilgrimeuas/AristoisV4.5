#include "SelfTrap.h"

#include "../../../Utils/Logger.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

SelfTrap::SelfTrap() : IModule(0x0, Category::PLAYER, "Trap Your Self") {
	mode = SettingEnum(this)
	.addEntry(EnumEntry("Obsidian", 0))
		.addEntry(EnumEntry("EndChest", 1))
		.addEntry(EnumEntry("BlackBrick", 2));
	registerEnumSetting("Mode", &mode, 0);
	registerBoolSetting("AntiCev", &this->sb, this->sb);
	registerBoolSetting("AirPlace", &this->airplace, this->airplace);
	registerBoolSetting("Expansion", &this->fullbody, this->fullbody);
	registerBoolSetting("Hold", &this->hold, this->hold);
}
SelfTrap::~SelfTrap() {
}

const char* SelfTrap::getModuleName() {
	return ("SelfTrap");
}
bool SelfTrap::isHoldMode() {
	return hold;
}
bool placeTrap(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}
void getobbbb() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void getec() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 130) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool spswap4(int Itemid) {
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
	return false;
}
bool stopsp1() {
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
	// C_MobEquipmentPacket a(id, 259, supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	// g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	// return;
	
}
void SelfTrap::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr))  // Block in hand?
		return;
	vec3_t EZ;
	EZ = g_Data.getLocalPlayer()->getHumanPos();  // Block below the player
	upper = EZ.add(0, 2, 0);
	//mid4 =  EZ.add(0, 3, 0);
	mid1 = EZ.add(0, 0, 1);
	mid2 = EZ.add(0, 0, -1);
	mid3 = EZ.add(-2, 0, 1);
	mid4 = EZ.add(-2, 0, -1);
	b1 = EZ.add(0, 0, 0);
	b2 = EZ.add(-2, 0, 0);
	b3 = EZ.add(-1, 0, 1);
	b4 = EZ.add(-1, 0, -1);

	if (mode.selected == 0) {
		spswap4(49);
	}

	if (mode.selected == 1) {
			spswap4(130);
		
	}

	if (mode.selected == 2) {
			spswap4(-270);
	}

	if (sb) {
			if (airplace) {
				gm->buildBlock(&vec3_ti(upper), 0, 3);
				gm->buildBlock(&vec3_ti(upper.add(0,1,0)), 0, 3);
				gm->buildBlock(&vec3_ti(b1), 0, 3);
				gm->buildBlock(&vec3_ti(b2), 0, 3);
				gm->buildBlock(&vec3_ti(b3), 0, 3);
				gm->buildBlock(&vec3_ti(b4), 0, 3);
				if (fullbody) {
				gm->buildBlock(&vec3_ti(mid1), 0, 3);
				gm->buildBlock(&vec3_ti(mid2), 0, 3);
				gm->buildBlock(&vec3_ti(mid3), 0, 3);
				gm->buildBlock(&vec3_ti(mid4), 0, 3);
				}
			}
			if (!airplace) {
				gm->buildBlock(&vec3_ti(upper), 0, 3);
				gm->buildBlock(&vec3_ti(upper.add(0, 1, 0)), 0, 3);
				placeTrap(b1);
				placeTrap(b2);
				placeTrap(b3);
				placeTrap(b4);
				if (fullbody) {
				placeTrap(mid4);
				placeTrap(mid3);
				placeTrap(mid2);
				placeTrap(mid1);
				}
			}
			stopsp1();
		
	} else if (airplace) {

			gm->buildBlock(&vec3_ti(b1), 0, 3);
			gm->buildBlock(&vec3_ti(b2), 0, 3);
			gm->buildBlock(&vec3_ti(b3), 0, 3);
			gm->buildBlock(&vec3_ti(b4), 0, 3);
			if (fullbody) {
				gm->buildBlock(&vec3_ti(mid1), 0, 3);
				gm->buildBlock(&vec3_ti(mid2), 0, 3);
				gm->buildBlock(&vec3_ti(mid3), 0, 3);
				gm->buildBlock(&vec3_ti(mid4), 0, 3);
			}
	}
	if (!airplace) {
			placeTrap(b1);
			placeTrap(b2);
			placeTrap(b3);
			placeTrap(b4);
			if (fullbody) {
				placeTrap(mid4);
				placeTrap(mid3);
				placeTrap(mid2);
				placeTrap(mid1);
			}
	}
	stopsp1();

}