#include "CevBreaker.h"
CevBreaker::CevBreaker() : IModule(0x0, Category::CPVP, "Damn and Shit CEV") {
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerIntSetting("Crytaldelay", &cdelay, cdelay, 0, 8);
	//registerIntSetting("Obsidiandelay", &odelay, odelay, 0, 8);
	registerIntSetting("Obsidiandelay", &pdelay, pdelay, 0, 8);
	registerIntSetting("Breakdelay", &bdelay, bdelay, 0, 8);
	registerBoolSetting("SilentSwitch", &silent, silent);
	registerBoolSetting("Airplace", &airplace, airplace);
	registerBoolSetting("StrongMod", &hasdone, hasdone);
}
CevBreaker::~CevBreaker() {
}

const char* CevBreaker::getModuleName() {
	return ("CevBreaker");
}

static std::vector<C_Entity*> jblist;

void FindJB(C_Entity* currentEntity, bool isRegularEntity) {
	static auto Cevmod = moduleMgr->getModule<CevBreaker>();
	if (currentEntity == nullptr)
		return;
	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;
	if (!g_Data.getLocalPlayer()->isAlive())
		return;
	if (!currentEntity->isAlive())
		return;
	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;
	if (!Target::isValidTarget(currentEntity))
		return;
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < Cevmod->range) {
		jblist.push_back(currentEntity);
	}
}
void Getob4() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 49)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select anvil
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void getCrystal4() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 637)
		return;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 637) {  // select netherite pick
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool doCrystal(vec3_t blkPlacement) {
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
			g_Data.getCGameMode()->buildBlock(&blok, i, 2);
			return true;
		}
	}
	return false;
}
void CevBreaker::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getItem()->getAttackDamage();
			if (currentDamage > damage && stack->getItem()->isPickaxe()) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
bool spoofget(int Itemid) {
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
bool stopspof() {
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
	bool consistent = false;
void CevBreaker::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	jblist.clear();

	g_Data.forEachEntity(FindJB);

	if (!jblist.empty()) {
		if (player == nullptr) return;
		bool idk = false;
		//auto playerOn = vec3_t(targetListXD[0]->getPos()->x, targetListXD[0]->aabb.lower.y + 0.1f, targetListXD[0]->getPos()->z);
		vec3_t onHead = (jblist[0]->eyePos0).floor();
		vec3_t SurPos = (jblist[0]->getHumanPos());
		pos = vec3_ti(onHead.add(0, 1, 0));
		pos1 = vec3_ti(SurPos.add(1, 1, -1));
		pos2 = vec3_ti(SurPos.add(1, 1, 1));
		pos3 = vec3_ti(SurPos.add(-1, 1, -1));
		pos4 = vec3_ti(SurPos.add(-1, 1, 1));
			if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
				if (!Clicked) {
					C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
					C_Inventory* inv = supplies->inventory;

						odelays = 0;
						gm->startDestroyBlock(pos, 0, idk);
						if (hasdone) {
					gm->startDestroyBlock(pos1, 0, idk);
					gm->startDestroyBlock(pos2, 0, idk);
					gm->startDestroyBlock(pos3, 0, idk);
					gm->startDestroyBlock(pos4, 0, idk);
						}
						idk = 2;
					Clicked = true;
				}
			}

			if (gm->player->region->getBlock(pos)->toLegacy()->blockId != 256) {
				if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
					C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
					C_Inventory* inv = supplies->inventory;

					bool idk = false;

					if (Clicked) {
						if (gm->player->region->getBlock(pos)->toLegacy()->blockId == 49 && !consistent) {
							odelays++;
							gm->player->level->rayHitType = 3;
							gm->player->level->rayHitVec = pos.toVec3t();
							if (odelays >= 28) {
								for (int n = 0; n < 9; n++) {
									C_ItemStack* stack = inv->getItemStack(n);
									if (stack->item != nullptr) {
				
										bool IsUseful =
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 5 ||
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 4 ||
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 3 ||
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 2 ||
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 1 ||
											stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 0;
									}
								}
							}

							if (odelays >= 28) {
								consistent = true;
								//hasdone = true;
							}
						}
						if (gm->player->region->getBlock(pos)->toLegacy()->blockId == 0) odelays = 0;

						if (consistent) {
							gm->destroyBlock(&pos, idk);
							gm->stopDestroyBlock(pos);
							if (hasdone) {
								gm->destroyBlock(&pos1, idk);
								gm->stopDestroyBlock(pos1);
								gm->destroyBlock(&pos2, idk);
								gm->stopDestroyBlock(pos2);
								gm->destroyBlock(&pos3, idk);
								gm->stopDestroyBlock(pos3);
								gm->destroyBlock(&pos4, idk);
								gm->stopDestroyBlock(pos4);
							}
						}

							if (!hasplaceob) {
								// NOT placed anchor
								if (!takenob) {
								if (silent)
									spoofget(49);
								if (!silent)
									Getob4();
									takenob = true;
									return;
								}

								
									gm->buildBlock(&pos, 0, 3);
								if (hasdone) {
									gm->buildBlock(&pos1, 0, 3);
									gm->buildBlock(&pos2, 0, 3);
									gm->buildBlock(&pos3, 0, 3);
									gm->buildBlock(&pos4, 0, 3);
								}
								
								
								
								hasplaceob = true;
									takenob = false;
								if (silent) {
									stopspof();
								}
								 //clientMessageF("Placed anchor!");
							}

							if (tickTimer >= pdelay && !Dhasob) {
								tickTimer = 0;
								Dhasob = true;
							} else if (tickTimer < pdelay && !Dhasob) {
								tickTimer++;
								return;
							}
							if (!hasplacecry) {
								if (!takencry) {
									if (silent)
									spoofget(637);
									if (!silent)
									getCrystal4();
									takencry = true;
									return;
								}

								
											gm->buildBlock(&pos, 0, 3);
								if (hasdone) {
									gm->buildBlock(&pos1, 0, 3);
									gm->buildBlock(&pos2, 0, 3);
									gm->buildBlock(&pos3, 0, 3);
									gm->buildBlock(&pos4, 0, 3);
								}

								hasplacecry = true;
								takencry = false;
								if (silent)
									stopspof();
								// clientMessageF("Charging!");
							}

							if (tickTimer >= cdelay && !DhascRY) {
								tickTimer = 0;
								DhascRY = true;
							} else if (tickTimer < cdelay && !DhascRY) {
								tickTimer++;
								return;
							}

							if (!hasbreak) {
								if (!takenpick) {
									findWeapon();
									// getAnchor2();
									takenpick = true;
									return;
								}
								g_Data.forEachEntity([](C_Entity* ent, bool b) {
									if (ent->getEntityTypeId() != 71)
										return;
									for (auto& i : jblist)
										if (ent->getPos()->dist(*i->getPos()) < 5) {
											g_Data.getCGameMode()->attack(ent);
											g_Data.getLocalPlayer()->swingArm();
											return;
										}
								});
								hasbreak = true;
							}
							if (tickTimer >= bdelay) {  // reset variables; prepare for another loop
								hasplaceob = false;
								hasplacecry = false;
								hasbreak = false;
								//hasdone = false;
								DhascRY = false;
								Dhasob = false;
								tickTimer = 0;
								takencry = false;
								takenob = false;
								takenpick = false;
								// stopSp();
								return;
							} else {
								tickTimer++;
								return;
							}
						}
					
				}
			}
		
	} else {
		odelays = 0;
		delay = 0;
		consistent = false;
		swaped = false;
		Clicked = false;
		pos = vec3_t(0, 0, 0);
		pos1 = vec3_t(0, 0, 0);
		pos2 = vec3_t(0, 0, 0);
		pos3 = vec3_t(0, 0, 0);
		pos4 = vec3_t(0, 0, 0);
	}
}
void CevBreaker::onEnable(){
	odelays = 0;
	delay = 0;
	consistent = false;
	swaped = false;
	Clicked = false;
	pos = vec3_t(0, 0, 0);
	pos1 = vec3_t(0, 0, 0);
	pos2 = vec3_t(0, 0, 0);
	pos3 = vec3_t(0, 0, 0);
	pos4 = vec3_t(0, 0, 0);
}
void CevBreaker::onSendPacket(C_Packet* packet) {
}
void CevBreaker::onPreRender(C_MinecraftUIRenderContext* context) {
	{
		float rentimer = 0;
		if (odelays != 0 && odelays <= 28) rentimer = (float)(odelays / 28.f);
		if (odelays >= 28) rentimer = 1;

		float zero = rentimer / 2;
		vec3_t block(pos.toVec3t().add(0.5));
		DrawUtils::setColor(0.f, 0.f, 1.f, 0.3f);
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