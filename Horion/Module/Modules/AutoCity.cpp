#include "AutoCity.h"

AutoCity::AutoCity() : IModule(0x0, Category::COMBAT, "Attacks entities around you automatically") {
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerIntSetting("Delay", &delay, delay, 0, 100);
	registerBoolSetting("SilentSwitch", &silent, silent);
}

AutoCity::~AutoCity() {
}

const char* AutoCity::getModuleName() {
	return ("AutoCity");
}

static std::vector<C_Entity*> targetListXD;
void findEntityBB(C_Entity* currentEntity, bool isRegularEntity) {
	static auto CityMod = moduleMgr->getModule<AutoCity>();

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

	if (dist < CityMod->range) {
		targetListXD.push_back(currentEntity);
	}
}

void AutoCity::findWeapon() {
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
std::vector<vec3_t> calcSideArray;
std::vector<vec3_t> playerSideArray;
bool isSideEmpty(C_Entity* ent) {
	bool sendBack = false;
	std::vector<vec3_t> sidesBlocksOnly = {
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1)};

	for (auto checkThis : sidesBlocksOnly) {
		auto playerOn = vec3_t(ent->getPos()->x, ent->aabb.lower.y + 0.1f, ent->getPos()->z);
		auto rndOffPlayerOn = playerOn.floor();

		auto calcSides = rndOffPlayerOn.add(checkThis);
		calcSideArray.push_back(calcSides);
		if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides))->toLegacy()->blockId == 0) {
			AABB hitbox = AABB(calcSides.add(0.05), calcSides.add(0.95));
			if (!hitbox.intersects(ent->aabb))
				sendBack = true;
		}
	}
	return sendBack;
}
bool fastbreak = false;
void AutoCity::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	// Loop through all our players and retrieve their information
	targetListXD.clear();

	g_Data.forEachEntity(findEntityBB);
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	{
		if (!targetListXD.empty()) {
			if (!isSideEmpty(targetListXD[0])) {
				auto player = g_Data.getLocalPlayer();
				if (player == nullptr) return;

				int minetime = 6;
				int swaptime = 2;
				float damage = 0;
				float speed = 0;
				int slot = -1;
				bool idk = false;
				std::vector<vec3_t> sidesBlocksOnly;
				std::vector<vec3_t> around = {
					vec3_t(1, 0, 0),
					vec3_t(0, 0, 1),
					vec3_t(-1, 0, 0),
					vec3_t(0, 0, -1)};
				for (auto& a : around) {
					/* if (gm->player->region->getBlock(vec3_t(targetListXD[0]->getPos()->x, targetListXD[0]->aabb.lower.y + 0.1f, targetListXD[0]->getPos()->z).floor().add(a))->blockLegacy->blockId == 49) {
						sidesBlocksOnly.push_back(vec3_t(targetListXD[0]->getPos()->x, targetListXD[0]->aabb.lower.y + 0.1f, targetListXD[0]->getPos()->z).floor().add(a));
						around.clear();
					}*/

					auto playerOn = vec3_t(targetListXD[0]->getPos()->x, targetListXD[0]->aabb.lower.y + 0.1f, targetListXD[0]->getPos()->z);
					auto rndOffPlayerOn = playerOn.floor();
					for (auto checkThis : around) {
						auto calcSides = rndOffPlayerOn.add(checkThis);
						if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides))->toLegacy()->blockId == 0) {
							AABB hitbox = AABB(calcSides.add(0.05), calcSides.add(0.95));
							if (hitbox.intersects(targetListXD[0]->aabb))
								rndOffPlayerOn = rndOffPlayerOn.add(checkThis);
						} else if (gm->player->region->getBlock(pos)->toLegacy()->blockId !=7 || 0) {
							sidesBlocksOnly.push_back(calcSides);
						}
					}
					for (auto checkThis : around) {
						auto calcSides = rndOffPlayerOn.add(checkThis);
						if (gm->player->region->getBlock(pos)->toLegacy()->blockId != 7 || 0) {
							sidesBlocksOnly.push_back(calcSides);
						}
					}
				}
				if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
					if (!hasClicked) {
						C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
						C_Inventory* inv = supplies->inventory;

						if (!sidesBlocksOnly.empty()) {
							odelays = 0;
							pos = sidesBlocksOnly[0];
							gm->startDestroyBlock(pos, 0, idk);
							face = 2;
						}
						hasClicked = true;
					}
					if (sidesBlocksOnly.empty()) return;
					if (pos != sidesBlocksOnly[0]) {  // if block chenged, disable fastbreak
						fastbreak = false;
						hasClicked = false;
					}
				}

				if (gm->player->region->getBlock(pos)->toLegacy()->blockId != 7 || 0) {
					if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
						C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
						C_Inventory* inv = supplies->inventory;

						float damage = 0;
						float speed = 0;
						int slot = -1;
						bool idk = false;

						if (hasClicked) {
							if ((gm->player->region->getBlock(pos)->toLegacy()->blockId != 7 || 0) && !fastbreak) {
								odelays++;
								gm->player->level->rayHitType = 3;
								gm->player->level->rayHitVec = pos.toVec3t();
								if (odelays >= 28) {
									for (int n = 0; n < 9; n++) {
										C_ItemStack* stack = inv->getItemStack(n);
										if (stack->item != nullptr) {
											float currentDamage = stack->getItem()->getAttackDamage();
											bool IsUseful =
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 5 ||
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 4 ||
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 3 ||
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 2 ||
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 1 ||
												stack->getItem()->getDestroySpeed(*stack, *g_Data.getLocalPlayer()->region->getBlock(pos)) == 0;
											if (currentDamage > damage && IsUseful) {
												damage = currentDamage;
												supplies->selectedHotbarSlot = n;
											}
										}
									}
								}

								if (odelays >= 28) {
									fastbreak = true;
								}
							}
							if (gm->player->region->getBlock(pos)->toLegacy()->blockId == 0) odelays = 0;

							if (fastbreak) {
								gm->destroyBlock(&pos, face);
								gm->stopDestroyBlock(pos);
							}
						}
					}
				}
			}
		} else {
			odelays = 0;
			fastbreak = false;
			swaped = false;
			hasClicked = false;
			pos = vec3_t(0, 0, 0);
		}
	}
}
void AutoCity::onEnable() {
	odelays = 0;
	fastbreak = false;
	swaped = false;
	hasClicked = false;
	pos = vec3_t(0, 0, 0);
}

void AutoCity::onSendPacket(C_Packet* packet) {
}
void AutoCity::onPreRender(C_MinecraftUIRenderContext* context) {
	{
		float rentimer = 0;
		if (odelays != 0 && odelays <= 28) rentimer = (float)(odelays / 28.f);
		if (odelays >= 28) rentimer = 1;

		float zero = rentimer / 2;
		vec3_t block(pos.toVec3t().add(0.5));
		DrawUtils::setColor(.3f, 8.f, .9f, 0.4f);
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
	} /*
	calcSideArray.clear();
	playerSideArray.clear();
	isSideEmpty(g_Data.getLocalPlayer());
	for (auto& calc : calcSideArray) {
		DrawUtils::setColor(0.f, 1.f, 0.f, 0.3f);
		DrawUtils::drawBox(calc, calc.add(1), 1.f, 1.f);
	}
	for (auto& calc : playerSideArray) {
		DrawUtils::setColor(1.f, 0.f, 0.f, 0.3f);
		DrawUtils::drawBox(calc, calc.add(1), 1.f, 1.f);
	}

*/
}