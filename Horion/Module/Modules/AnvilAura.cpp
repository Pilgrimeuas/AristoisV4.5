#include "AnvilAura.h"
#include "../../../SDK/Tag.h"

AnvilAura::AnvilAura() : IModule(VK_NUMPAD0, Category::CPVP, "Get black \"people\" out of your mom's hole") {
	registerIntSetting("Range", &this->AArange, this->AArange, 1, 7);
	registerBoolSetting("helmetBreaker", &this->hBreak, this->hBreak);
	registerBoolSetting("airPlace", &this->airplace, this->airplace);
    registerIntSetting("Switch Delay", &this->maxSwitchDelay, this->maxSwitchDelay, 0, 25);

}
AnvilAura::~AnvilAura() {
}
const char* AnvilAura::getModuleName() {
	return ("AnvilAura");
}

static std::vector<C_Entity*> targetList;

void findTargEntity(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return ;
	if (curEnt == g_Data.getLocalPlayer()) return ;  // Skip Local player
	if (!curEnt->isAlive()) return ;
	if (!g_Data.getLocalPlayer()->isAlive()) return ;
	if (curEnt->getEntityTypeId() == 71) return ;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return ;  // falling block
	if (curEnt->getEntityTypeId() == 64) return ;  // item
	if (curEnt->getEntityTypeId() == 69) return ;  // xp orb
	if (!Target::isValidTarget(curEnt)) return ;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < moduleMgr->getModule<AnvilAura>()->AArange) {
		targetList.push_back(curEnt);
	}
}

bool placeBlok(vec3_t blkPlacement) {
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

void getAnvil() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 145)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 145) {  // select anvil
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool spswap9(int Itemid) {
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
bool stopsp9() {
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
void getPickAxe() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 604) {  // select netherite pick
				supplies->selectedHotbarSlot = n;
				return;
			}
			if (stack->getItem()->itemId == 318){    // select diamond pick
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
/*
int getHeightUnderHole(vec3_t locFromPos) { // ok i think this code is shit so i won't use it
	locFromPos = locFromPos.floor();
	int h = 0;

	while (true) {
		// check blocks along x axis 
		if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(locFromPos.add(1, h, 0)))->toLegacy()->blockId == 0 &&
			g_Data.getLocalPlayer()->region->getBlock(vec3_ti(locFromPos.add(-1, h, 0)))->toLegacy()->blockId == 0) {	
			h += 1;
			continue;
		}
		// check blocks along y axis
		else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(locFromPos.add(0, h, 1)))->toLegacy()->blockId == 0 &&
				 g_Data.getLocalPlayer()->region->getBlock(vec3_ti(locFromPos.add(0, h, -1)))->toLegacy()->blockId == 0) {
			h += 1;
			continue;
		}
		else break;
	}
	return h;
}
*/
void AnvilAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	targetList.clear();
	if (hBreak)return;
	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	//getAnvil();
}

bool AnvilAura::isSidesAllAir(vec3_t locFromPos) {
	bool sendBack = false;
	auto region = g_Data.getLocalPlayer()->region;

	for (auto checkThis : sidesBlocksOnly) {
		auto playerOn = g_Data.getLocalPlayer()->getPos();
		auto rndOffPlayerOn = playerOn->floor();
		rndOffPlayerOn.y -= 1.f;

		auto calcSides = rndOffPlayerOn.add(checkThis);
		if (region->getBlock(vec3_ti(locFromPos))->toLegacy()->blockId == 0) {
			sendBack = true;
		}
	}
	if (sendBack == true) {
		return true;
	} else {
		return false;
	}
}

void AnvilAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	g_Data.forEachEntity(findTargEntity);
	spswap9(145);
	if (!targetList.empty()) {
		for (auto& targThis : targetList) {  // this makes it attack multiple enemies

			vec3_t onHead = (targThis->eyePos0).floor();
			onHead.y += targThis->height + 1;

			if (!hBreak) {
				for (int c = 0; c <= 1; c++) {  // lmao c++

					if (airplace)
						gm->buildBlock(&vec3_ti(onHead), 0, true);
					else if (!airplace && !isSidesAllAir(onHead)) {
						placeBlok(onHead);
					}
				}
				stopsp9();
				moduleMgr->getModule<AnvilAura>()->toggle();
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlot;

			} 
			else if (hBreak) {

				if (!anvilInHand) {
					spswap9(145);
					anvilInHand = true;
					return;
				}

				if (!placedAnvil) {
					if (airplace)
						gm->buildBlock(&vec3_ti(onHead), 0, true);
					else if (!airplace && !isSidesAllAir(onHead)) {
						placeBlok(onHead);
					}
					
					placedAnvil = true;
					
					
				}

				if (switchDelay >= maxSwitchDelay) {
					if (!pickInHand) {
						getPickAxe();
						pickInHand = true;
						return;
					}

					vec3_t onFeet = onHead.add(0, -2, 0);

					bool isDestroyed = false;

					g_Data.getCGameMode()->startDestroyBlock(onFeet, 0, isDestroyed);
					g_Data.getCGameMode()->stopDestroyBlock(onFeet);

					if (isDestroyed && g_Data.getLocalPlayer()->region->getBlock(onFeet)->blockLegacy->blockId != 0) {
						g_Data.getCGameMode()->destroyBlock(&vec3_ti(onFeet), 0);
					}
					placedAnvil = false;
					anvilInHand = false;
					pickInHand = false;
					switchDelay = 0;
					return;
				} else {
					switchDelay++;
					return;
				}

			}
		}
	}
}
	
