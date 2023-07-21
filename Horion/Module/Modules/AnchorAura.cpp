#include "AnchorAura.h"

#include "../../SDK/Tag.h"

AnchorAura::AnchorAura() : IModule(VK_NUMPAD0, Category::CPVP, "Improved By Dimpless") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("anchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	this->registerIntSetting("gsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	this->registerIntSetting("Break delay", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("strongmod", &this->airplace, this->airplace);
	this->registerBoolSetting("spoof", &this->spoof, this->spoof);
}
AnchorAura::~AnchorAura() {
}
const char* AnchorAura::getModuleName() {
	return ("AnchorAura");
}

static std::vector<C_Entity*> targetList;

void AnchorAura::onEnable() {
	targetList.clear();
}

bool Findentbyc(C_Entity* curEnt, bool isRegularEntity) {
	std::string Enzo = "Enzo6172";
	std::string hell = "hellorogic";
	std::string TargetUtilName = curEnt->getNameTag()->getText();
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<AnchorAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}



void getAnchor2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == -272)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == -272) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void getGS2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 89)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 89) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool spswap(int Itemid) {
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
bool stopsp() {
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
bool invcheck(int Itemid) {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId != 0) {
				if (stack->getItem()->itemId == Itemid) {
					return true;
				}
			}
		}
	}
	return false;
}
void AnchorAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	targetList.clear();
	g_Data.forEachEntity(Findentbyc);

	int place = 0;

	if (!targetList.empty()) {
		for (auto& i : targetList) {
			vec3_t enemyLoc = (i->getHumanPos().floor());
			bottom2 = enemyLoc.add(1, 1, 0);
			mid1 = enemyLoc.add(0, 2, 0);
			bottom3 = enemyLoc.add(-1, 1, 0);
			bottom4 = enemyLoc.add(0, 1, 1);
			bottom1 = enemyLoc.add(0, 1, -1);
			pos = mid1;
			pos1 = bottom2;
			pos2 = bottom3;
			pos3 = bottom4;
			pos4 = bottom1;
			if (!hasPlacedAnchor) {
				// NOT placed anchor
				if (!takenAnchor) {
					if (spoof)
						spswap(-272);
					if (!spoof)
						getAnchor2();
					takenAnchor = true;
					return;
				}

				if (g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
					for (auto& i : targetList)
						if (airplace) {
							anchordelay++;
							if (anchordelay = 10)
								gm->buildBlock(&vec3_ti(pos), 0, 3);
							if (anchordelay = 20)
								gm->buildBlock(&vec3_ti(pos1), 0, 3);
							if (anchordelay = 30)
								gm->buildBlock(&vec3_ti(pos2), 0, 3);
							if (anchordelay = 40)
								gm->buildBlock(&vec3_ti(pos3), 0, 3);
							if (anchordelay = 50)
								gm->buildBlock(&vec3_ti(pos4), 0, 3);
						} else {
							gm->buildBlock(&vec3_ti(pos), 0, 3);
						}
						}
				Option = 1;
				}

				hasPlacedAnchor = true;
				// clientMessageF("Placed anchor!");
			}

			if (tickTimer >= 2 && !DhasPlacedAnchor) {
				if (spoof)
				stopsp();
				tickTimer = 0;
				DhasPlacedAnchor = true;
			} else if (tickTimer < 2 && !DhasPlacedAnchor) {
				tickTimer++;
				return;
			}

			if (!hasCharged) {
				if (!takenGS) {
				 if (spoof)
					spswap(89);
				 if (!spoof)
					getGS2();
					takenGS = true;
					return;
				}

				switch (Option) {
				case 1:

					bool sb = g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId != 0;
					bool sb2 = g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId != 0;
					if (sb || sb2) {
					if (airplace) {
						anchordelay++;
						if (anchordelay = 10)
							gm->buildBlock(&vec3_ti(pos), 0, 3);
						if (anchordelay = 20)
							gm->buildBlock(&vec3_ti(pos1), 0, 3);
						if (anchordelay = 30)
							gm->buildBlock(&vec3_ti(pos2), 0, 3);
						if (anchordelay = 40)
							gm->buildBlock(&vec3_ti(pos3), 0, 3);
						if (anchordelay = 50)
							gm->buildBlock(&vec3_ti(pos4), 0, 3);
					} else {
						gm->buildBlock(&vec3_ti(pos), 0, 3);
					}
						}

					break;
				}


				hasCharged = true;
				takenAnchor = false;
				anchordelay = 0;
				// clientMessageF("Charging!");
			}

			if (tickTimer >= gsdelay && !DhasCharged) {
				if (spoof)
					stopsp();
				tickTimer = 0;
				DhasCharged = true;
			} else if (tickTimer < gsdelay && !DhasCharged) {
				tickTimer++;
				return;
			}

			if (!takenAnchor) {
				if (spoof)
					spswap(-272);
				if (!spoof)
					getAnchor2();
				// getAnchor2();
				takenAnchor = true;
				return;
			}

			if (!hasDetonated) {
		
				switch (Option) {
				case 1:
					if (airplace) {
					anchordelay++;
					if (anchordelay = 10)
						gm->buildBlock(&vec3_ti(pos), 0, 3);
					if (anchordelay = 20)
						gm->buildBlock(&vec3_ti(pos1), 0, 3);
					if (anchordelay = 30)
						gm->buildBlock(&vec3_ti(pos2), 0, 3);
					if (anchordelay = 40)
						gm->buildBlock(&vec3_ti(pos3), 0, 3);
					if (anchordelay = 50)
						gm->buildBlock(&vec3_ti(pos4), 0, 3);
					} else {
					gm->buildBlock(&vec3_ti(pos), 0, 3);
					}
					break;
					anchordelay = 0;
				hasDetonated = true;
			}
			// clientMessageF("Detonated!");

			if (tickTimer >= breakdelay) {
				if (spoof)
					stopsp();  // reset variables; prepare for another loop
				hasPlacedAnchor = false;
				hasCharged = false;
				hasDetonated = false;
				DhasPlacedAnchor = false;
				DhasCharged = false;
				tickTimer = 0;
				takenAnchor = false;
				takenGS = false;
				// stopSp();
				return;
			} else {
				tickTimer++;
				return;
			}
		}
	}
}
void AnchorAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	{
		odelays--;
				float rentimer = 1;
		if (odelays != 0 && odelays <= 100) rentimer = (float)(odelays / 100.f);
				if (odelays == 0) {
			rentimer = 0;
			odelays =100;
				}

		float zero = rentimer / 2;
				vec3_t block(pos.toVec3t().add(0.5));
		DrawUtils::setColor(0.f, 3.f, 0.f, 0.3f);
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
				{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
				}
	}
}

void AnchorAura::onDisable() {
}
	/*if (!hasPlacedAnchor) {
				// NOT placed anchor
				if (!takenAnchor) {
						getAnchor2();
					takenAnchor = true;
					return;
				}

				if (g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
					for (auto& i : targetList)
	
							gm->buildBlock(&vec3_ti(pos), 0, 3);
						}
				Option = 1;
				}
				// stopSp();
				hasPlacedAnchor = true;
				// clientMessageF("Placed anchor!");
			}

			if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
				tickTimer = 0;
				DhasPlacedAnchor = true;
			} else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
				tickTimer++;
				return;
			}

			if (!hasCharged) {
				if (!takenGS) {
						getGS2();
					takenGS = true;
					return;
				}

				switch (Option) {
				case 1:

					bool sb = g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId != 0;
					bool sb2 = g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId != 0;
					if (sb || sb2) {
							gm->buildBlock(&vec3_ti(pos), 0, 3);
						}
					
					break;
				}


				hasCharged = true;
				takenAnchor = false;
				// clientMessageF("Charging!");
			}

			if (tickTimer >= gsdelay && !DhasCharged) {
				tickTimer = 0;
				DhasCharged = true;
			} else if (tickTimer < gsdelay && !DhasCharged) {
				tickTimer++;
				return;
			}

			if (!takenAnchor) {
					getAnchor2();
				// getAnchor2();
				takenAnchor = true;
				return;
			}

			if (!hasDetonated) {
				switch (Option) {
				case 1:
					gm->buildBlock(&vec3_ti(pos), 0, 4);
					break;
				

				hasDetonated = true;
			}
			// clientMessageF("Detonated!");

			if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
				hasPlacedAnchor = false;
				hasCharged = false;
				hasDetonated = false;
				DhasPlacedAnchor = false;
				DhasCharged = false;
				tickTimer = 0;
				takenAnchor = false;
				takenGS = false;
				// stopSp();
				return;
			} else {
				tickTimer++;
				return;
			}
		}
	}
}*/