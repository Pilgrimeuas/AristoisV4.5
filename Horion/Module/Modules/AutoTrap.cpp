#include "AutoTrap.h"

#include "../../../Utils/Logger.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

AutoTrap::AutoTrap() : IModule(0x0, Category::CPVP, "Automatically traps the nearest player") {
	this->registerIntSetting("range", &this->range, this->range, 3, 12);
	registerBoolSetting("onClick", &this->onClick, this->onClick);
	registerBoolSetting("PitchUp", &this->ATRots, this->ATRots);
	registerBoolSetting("Full body", &this->fullbody, this->fullbody);
	registerBoolSetting("Auto CA", &this->niggerTurnCAOn, this->niggerTurnCAOn);
	registerBoolSetting("SilentSwap", &this->switch2obby, this->switch2obby);
	registerBoolSetting("Airplace", &this->airplace, this->airplace);
}

AutoTrap::~AutoTrap() {
}

const char* AutoTrap::getModuleName() {
	return ("AutoTrap");
}
bool spswap8(int Itemid) {
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
bool stopsp8() {
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

static std::vector<C_Entity*> targetList15;

void findEntity15(C_Entity* currentEntity15, bool isRegularEntity) {
	static auto AutoTrapMod = moduleMgr->getModule<AutoTrap>();

	if (currentEntity15 == nullptr)
		return;

	if (currentEntity15 == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity15, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity15->isAlive())
		return;

	if (!Target::isValidTarget(currentEntity15))
		return;

	float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < AutoTrapMod->range) {
		targetList15.push_back(currentEntity15);

		float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < AutoTrapMod->range) {
			targetList15.push_back(currentEntity15);
		}
	}
}

bool AutoTrap::tryAutoTrap(vec3_t blkPlacement) {
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

void AutoTrap::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	bool mustGoUp = false;
	placements.clear();

	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	targetList15.clear();
	g_Data.forEachEntity(findEntity15);

	int place = 1;

	if (onClick) {
		if (GameData::isRightClickDown()) {
			place = 0;
		} else {
			place = 1;
		}
	}

	if (!onClick) {
		place = 0;
	}

	if (switch2obby)
		spswap8(49);

	if (!targetList15.empty()) {
		//ground level
		vec3_t enemLoc = (targetList15[0]->eyePos0).floor();

		if (!fullbody) {
			/* GROUND   FEET     HEAD
	       0 0 N	0 0 N    N N N    +x
	       0 0 0    0 0 0    N 0 N  -z   +z
	       0 0 0	0 0 0    0 N N	  -x
			*/
			placements.clear();
			placements.push_back(enemLoc.add(1, 0, 1) );
			placements.push_back(enemLoc.add(1, -1, 1)) ;
			placements.push_back(enemLoc.add(1, 1, 1) );
			placements.push_back(enemLoc.add(0, 1, 1) );
			placements.push_back(enemLoc.add(-1, 1, 1)) ;
			placements.push_back(enemLoc.add(-1, 1, 0)) ;
			placements.push_back(enemLoc.add(1, 1, 0) ) ;
			placements.push_back(enemLoc.add(1, 1, -1)) ;
			placements.push_back(enemLoc.add(0, 1, -1)) ;
		} else {
			/*
			GROUND   FEET     HEAD 
			0 N 0	 0 N 0	  0 N 0     +x
			N 0 N	 N 0 N	  N N N	  -z   +z
			0 N 0 	 0 N 0	  0 N 0	    -x
			*/
			placements.clear();

			placements.push_back(enemLoc.add(1, -1, 0));
			placements.push_back(enemLoc.add(-1, -1, 0));
			placements.push_back(enemLoc.add(0, -1, 1));
			placements.push_back(enemLoc.add(0, -1, -1));
			
			placements.push_back(enemLoc.add(1, 0, 0));
			placements.push_back(enemLoc.add(-1,0, 0));
			placements.push_back(enemLoc.add(0, 0, 1));
			placements.push_back(enemLoc.add(0, 0, -1));

			placements.push_back(enemLoc.add(1, 1, 0));
			placements.push_back(enemLoc.add(-1,1, 0));
			placements.push_back(enemLoc.add(0, 1, 1));
			placements.push_back(enemLoc.add(0, 1, -1));

			placements.push_back(enemLoc.add(1, 2, 0));
			placements.push_back(enemLoc.add(0, 2, 0));
		}

		if (place == 0) {
			if (ATRots)
				mustGoUp = true;

			for (vec3_t i : placements) {
				if (airplace)
					tryAutoTrap(i);
				else
					gm->buildBlock(&vec3_ti(i), 0, true);
			}

			if (niggerTurnCAOn) {
				auto CBreak = moduleMgr->getModule<CrystalPlace>();
				if (!CBreak->isEnabled())
					CBreak->setEnabled(true);

				placements.clear();
				this->setEnabled(false);
			}
		}
	}
	if (switch2obby)
		stopsp8();
	placements.clear();
}
