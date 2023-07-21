#include "NewBedAura.h"

#include "../../SDK/Tag.h"

NewBedAura::NewBedAura()  : IModule(VK_NUMPAD0, Category::CPVP, "LOL NewBedAura") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("1Delay", &this->pdelay, this->pdelay, 0, 30);
	this->registerIntSetting("2Delay", &this->trapdelay, this->trapdelay, 0, 30);
	this->registerIntSetting("3Delay", &this->rdelay, this->rdelay, 0, 30);
	this->registerIntSetting("1Delay 4", &this->cdelay, this->cdelay, 0, 30);
	this->registerIntSetting("2Delay 0", &this->cbrange, this->cbrange, 0, 10);
	this->registerIntSetting("3Delay 0", &this->breakdelay, this->breakdelay, 0, 30);
	this->registerIntSetting("best cfg", &this->crange, this->crange, 0, 10);
	 this->registerBoolSetting("spoof", &this->spoof, this->spoof);
	//this->registerBoolSetting("High Level", &this->airplace, this->airplace);
	//this->registerBoolSetting("Bypass", &this->bypass, this->bypass);
	//this->registerBoolSetting("North", &this->n2, this->n2);
	//this->registerBoolSetting("South", &this->s2, this->s2);
	//this->registerBoolSetting("East", &this->e2, this->e2);
	//this->registerBoolSetting("West", &this->w2, this->w2);
}
NewBedAura::~NewBedAura() {
}
const char* NewBedAura::getModuleName() {
	return ("NewBedAura");
}

static std::vector<C_Entity*> targetList;

void NewBedAura::onEnable() {
	targetList.clear();
}

bool findEntByCC51(C_Entity* curEnt, bool isRegularEntity) {
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
	if (dist <= moduleMgr->getModule<NewBedAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}

bool placeBloKKK279(vec3_t blkPlacement) {
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

void getAnchor53() {  // get RedStone id 0
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 33 || g_Data.getLocalPlayer()->getSelectedItemId() == 418)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 33 || stack->getItem()->itemId == 418) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void getGS53() {  // get Piston 418  //77 is button //143 is button
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 0 || g_Data.getLocalPlayer()->getSelectedItemId() == 607 || g_Data.getLocalPlayer()->getSelectedItemId() == 607 || g_Data.getLocalPlayer()->getSelectedItemId() == 607 || g_Data.getLocalPlayer()->getSelectedItemId() == 395 || g_Data.getLocalPlayer()->getSelectedItemId() == 396 || g_Data.getLocalPlayer()->getSelectedItemId() == 397 || g_Data.getLocalPlayer()->getSelectedItemId() == 398 || g_Data.getLocalPlayer()->getSelectedItemId() == 399)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 0 || stack->getItem()->itemId == 607 || stack->getItem()->itemId == 607 || stack->getItem()->itemId == 607 || stack->getItem()->itemId == 395 || stack->getItem()->itemId == 396 || stack->getItem()->itemId == 397 || stack->getItem()->itemId == 398 || stack->getItem()->itemId == 399) {  // select redstone or more
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool spswap53(int Itemid) {
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
bool stopsp53() {
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
bool invcheck53(int Itemid) {
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
void NewBedAura::onTick(C_GameMode* gm) {
	static auto Reachmod = moduleMgr->getModule<Reach>();
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	targetList.clear();
	g_Data.forEachEntity(findEntByCC51);
	int place = 0;
	/* X 360 X   315-45= s
  X X X     45-135 = e
  X X X     135-225 = n   1 =
			225-315 = w*/
	int look = (int)(player->yaw + 180);
	//		if (moduleMgr->getModule<NewBedAura>()->lookat.selected != 0)
	// moduleMgr->getModule<NewBedAura>()->lookat.selected = 0;
	if (look >= 45 && look <= 135) {
		if (!moduleMgr->getModule<NewBedAura>()->w2)
			moduleMgr->getModule<NewBedAura>()->w2 = true;  // w
		moduleMgr->getModule<NewBedAura>()->s2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->e2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->n2 = false;     // e
	}
	if (look >= 135 && look <= 225) {
		if (!moduleMgr->getModule<NewBedAura>()->s2)        /// s
			moduleMgr->getModule<NewBedAura>()->s2 = true;  // e
		moduleMgr->getModule<NewBedAura>()->e2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->w2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->n2 = false;     // e
	}
	if (look >= 225 && look <= 315) {
		if (!moduleMgr->getModule<NewBedAura>()->e2)        // e
			moduleMgr->getModule<NewBedAura>()->e2 = true;  // e
		moduleMgr->getModule<NewBedAura>()->s2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->w2 = false;     // e
		moduleMgr->getModule<NewBedAura>()->n2 = false;     // e
	}
	if ((look >= 0 && look <= 45) || (look >= 315 && look <= 360)) {
		if (!moduleMgr->getModule<NewBedAura>()->n2)         // n
			moduleMgr->getModule<NewBedAura>()->e2 = false;  // e
		moduleMgr->getModule<NewBedAura>()->s2 = false;      // e
		moduleMgr->getModule<NewBedAura>()->w2 = false;      // e
		moduleMgr->getModule<NewBedAura>()->n2 = true;       // e
	}

	if (!targetList.empty()) {
		Reachmod->reachValue = 0;
		if (!hasPlacedAnchor) {  // NOT placed anchor
			if (!takenAnchor) {
				if (spoof)
					spswap53(33);
				spswap53(418);
				if (!spoof)
					getAnchor53();  // getpiston
				takenAnchor = true;
				return;
			}
			vec3_t enemyLoc = (targetList[0]->eyePos0).floor();
			w = enemyLoc.add(2, 1, 0);
			e = enemyLoc.add(-2, 1, 0);
			s = enemyLoc.add(0, 1, 2);
			n = enemyLoc.add(0, 1, -2);
			// restone2
			/* X 360 X   315-45= s
			 *  X X X     45-135 = e
			 *  X X X     135-225 = n
			 *            225-315 = w
			 */
			rw = enemyLoc.add(3, 0, 0);
			re = enemyLoc.add(-3, 0, 0);
			rs = enemyLoc.add(0, 0, 3);
			rn = enemyLoc.add(0, 0, -3);
			// top level

			neckBreaker = enemyLoc.add(0, 1, 0);
			// middle level  ~ last priority because it is the least effective
			//
			// piston
			w1 = enemyLoc.add(2, 0, 0);
			e1 = enemyLoc.add(-2, 0, 0);
			s1 = enemyLoc.add(0, 0, 2);
			n1 = enemyLoc.add(0, 0, -2);
			// crystal
			cw = enemyLoc.add(1, 0, 0);
			ce = enemyLoc.add(-1, 0, 0);
			cs = enemyLoc.add(0, 0, 1);
			cn = enemyLoc.add(0, 0, -1);
			std::vector<vec2_t> stacking{};
			/*				auto rncheck = g_Data.getLocalPlayer()->region->getBlock(cw)->toLegacy()->blockId;
				if (rncheck == 49 || rncheck == 7)
					airplace = true;
				else
					airplace = false;*/

			//�����ж�

			if (w2) {  // do piston
				C_Block* block = gm->player->region->getBlock(cw);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				} else {
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(ce), 0, true);
				else
					gm->buildBlock(&vec3_ti(ce), 0, true);
				Option = 1;
			}

			if (e2) {
				C_Block* block = gm->player->region->getBlock(ce);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				} else {
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(cw), 0, true);
				else
					gm->buildBlock(&vec3_ti(cw), 0, true);
				Option = 2;
			}

			if (s2) {
				C_Block* block = gm->player->region->getBlock(cs);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				} else {
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(cn), 0, true);
				else
					gm->buildBlock(&vec3_ti(cn), 0, true);
				Option = 3;
			}

			if (n2) {
				C_Block* block = gm->player->region->getBlock(cn);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				} else {
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(cs), 0, true);
				else
					gm->buildBlock(&vec3_ti(cs), 0, true);
				Option = 4;
			}
			hasPlacedAnchor = true;
		}
		
	}

	if (tickTimer >= rdelay && !DhasPlacedAnchor) {  // rdelay = pistondelay
		tickTimer = 0;
		DhasPlacedAnchor = true;
	} else if (tickTimer < rdelay && !DhasPlacedAnchor) {
		tickTimer++;
		return;
	}

	if (!hasDetonated) {
		if (!takenRS) {  // redstone
			if (spoof)
				spswap53(143);
			spswap53(0);
			if (!spoof)
				getGS53();  // getpiston
			takenRS = true;
			return;
		}
		switch (Option) {
		case 1:

			if (airplace)
				gm->buildBlock(&vec3_ti(ce.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(ce), 0, true);
			break;
		case 2:
			if (airplace)
				gm->buildBlock(&vec3_ti(cw.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cw), 0, true);
			break;
		case 3:
			if (airplace)
				gm->buildBlock(&vec3_ti(cn.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cn), 0, true);
			break;
		case 4:
			if (airplace)
				gm->buildBlock(&vec3_ti(cs.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cs), 0, true);
			break;
		}
		hasDetonated = true;
	}
	if (tickTimer >= pdelay && !DhasDetonated) {
		tickTimer = 0;
		DhasDetonated = true;
	} else if (tickTimer < pdelay && !DhasDetonated) {
		tickTimer++;
		return;
	}
	if (!hasbb) {
		switch (Option) {
		case 1:
			if (airplace)
				gm->buildBlock(&vec3_ti(ce.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(ce), 0, true);
			break;
		case 2:
			if (airplace)
				gm->buildBlock(&vec3_ti(cw.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cw), 0, true);
			break;
		case 3:
			if (airplace)
				gm->buildBlock(&vec3_ti(cn.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cn), 0, true);
			break;
		case 4:
			if (airplace)
				gm->buildBlock(&vec3_ti(cs.add(0, 1, 0)), 0, true);
			else
				gm->buildBlock(&vec3_ti(cs), 0, true);
			break;
		}
		hasbb = true;
		Reachmod->reachValue = 10;
	}
	if (tickTimer >= pdelay && !Dhasbb) {
		tickTimer = 0;
		Dhasbb = true;
	} else if (tickTimer < pdelay && !Dhasbb) {
		tickTimer++;
		return;
	}
	

	if (tickTimer >= breakdelay) { 
		Reachmod->reachValue = 10;  // NOT placed anchor// reset variables; prepare for another loop
		hasPlacedAnchor = false;
		hasCharged = false;
		hasbreak = false;
		hasDetonated = false;
		hasbb = false;
		hastrap = false;
		//
		DhasPlacedAnchor = false;
		DhasCharged = false;
		DhasDetonated = false;
		Dhasbb = false;
		Dtrap = false;
		tickTimer = 0;
		//
		takenAnchor = false;
		takenRS = false;
		takenGS = false;
		takentrap = false;
		anti = false;
		//this->setEnabled(false);
		return;
	} else {
		tickTimer++;
		return;
	}
}

void NewBedAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	// DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
	// DrawUtils::drawBox(currentBlock.toVec3t(), currentBlock.toVec3t().add(1), .4f, false);
	//
	if (w2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rw.add(0, 1, 0)).toVec3t(), vec3_ti(rw.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cw.add(0, 1, 0)).toVec3t(), vec3_ti(cw.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(w1.add(0, 1, 0)).toVec3t(), vec3_ti(rw.add(0, 1, 0)).toVec3t().add(1), .4f, true);
		} else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rw).toVec3t(), vec3_ti(rw).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cw).toVec3t(), vec3_ti(cw).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(w1).toVec3t(), vec3_ti(w1).toVec3t().add(1), .4f, true);
		}
	} else if (e2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(re.add(0, 1, 0)).toVec3t(), vec3_ti(re.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(ce.add(0, 1, 0)).toVec3t(), vec3_ti(ce.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(e1.add(0, 1, 0)).toVec3t(), vec3_ti(e1.add(0, 1, 0)).toVec3t().add(1), .4f, true);
		} else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(re).toVec3t(), vec3_ti(re).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(ce).toVec3t(), vec3_ti(ce).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(e1).toVec3t(), vec3_ti(e1).toVec3t().add(1), .4f, true);
		}
	} else if (s2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rs.add(0, 1, 0)).toVec3t(), vec3_ti(rs.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cs.add(0, 1, 0)).toVec3t(), vec3_ti(cs.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(s1.add(0, 1, 0)).toVec3t(), vec3_ti(s1.add(0, 1, 0)).toVec3t().add(1), .4f, true);
		} else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rs).toVec3t(), vec3_ti(rs).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cs).toVec3t(), vec3_ti(cs).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(s1).toVec3t(), vec3_ti(s1).toVec3t().add(1), .4f, true);
		}
	} else if (n2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rn.add(0, 1, 0)).toVec3t(), vec3_ti(rn.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cn.add(0, 1, 0)).toVec3t(), vec3_ti(cn.add(0, 1, 0)).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(n1.add(0, 1, 0)).toVec3t(), vec3_ti(n1.add(0, 1, 0)).toVec3t().add(1), .4f, true);
		} else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(rn).toVec3t(), vec3_ti(rn).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(cn).toVec3t(), vec3_ti(cn).toVec3t().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 0.f);  // 138,27,255
			DrawUtils::drawBox(vec3_ti(n1).toVec3t(), vec3_ti(n1).toVec3t().add(1), .4f, true);
		}
	}
}

void NewBedAura::onDisable() {
	tickTimer = 0;
	hasPlacedAnchor = false;
	hasCharged = false;
	hasbreak = false;
	hasDetonated = false;
	hasbb = false;
	hastrap = false;
	//
	DhasPlacedAnchor = false;
	DhasCharged = false;
	DhasDetonated = false;
	Dhasbb = false;
	Dtrap = false;
	tickTimer = 0;
	//
	takenAnchor = false;
	takenRS = false;
	takenGS = false;
	takentrap = false;
	anti = false;
}
void NewBedAura::onLevelRender() {
}