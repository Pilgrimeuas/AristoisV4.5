#include "FuckHead.h"
#include "Module.h"
#include "../../../SDK/Tag.h"
#include "../ModuleManager.h"

FuckHead::FuckHead() : IModule(VK_NUMPAD0, Category::CPVP, "Fuckhead By Dimpless") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	// this->registerIntSetting("anchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	// this->registerIntSetting("gsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	// this->registerIntSetting("Break delay", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("PuaseOnGapple", &this->airplace, this->airplace);
	this->registerBoolSetting("Spoof", &this->spoof, this->spoof);
	this->registerBoolSetting("OnlyHead", &this->only, this->only);
}
FuckHead::~FuckHead() {
}
const char* FuckHead::getModuleName() {
	return ("FuckHead");
}

static std::vector<C_Entity*> targetList;

void FuckHead::onEnable() {
	targetList.clear();
}
bool Findsb(C_Entity* curEnt, bool isRegularEntity) {
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
	if (dist <= moduleMgr->getModule<FuckHead>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}
bool FuckHead::obspoof() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 49) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	// C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	// g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}
void stopSp2() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}

void obget() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 49)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void FuckHead::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 607)
		return;
	if (airplace)
		if (g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && g_Data.isRightClickDown()) return;
	targetList.clear();
	g_Data.forEachEntity(Findsb);

	if (!targetList.empty()) {
		for (auto& i : targetList) {
			vec3_t enemyLoc = (i->eyePos0).floor();
			bottom2 = enemyLoc.add(1, 0, 0);
			mid1 = enemyLoc.add(0, 1, 0);
			bottom3 = enemyLoc.add(-1, 0, 0);
			bottom4 = enemyLoc.add(0, 0, 1);
			bottom1 = enemyLoc.add(0, 0, -1);
			if (spoof) {
				obspoof();
			} else {
				obget();
			}
			if (g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
				for (auto& i : targetList)
					if (only) {
						gm->buildBlock(&vec3_ti(mid1), 0,1);
					} else {
						gm->buildBlock(&vec3_ti(mid1), 0,1);
						gm->buildBlock(&vec3_ti(bottom1), 0,1);
						gm->buildBlock(&vec3_ti(bottom2), 0,1);
						gm->buildBlock(&vec3_ti(bottom3), 0,1);
						gm->buildBlock(&vec3_ti(bottom4), 0,1);
					}
			}
			if (spoof)
				stopSp2();
		}
	}
}
void FuckHead::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (only) {
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(mid1).toVec3t(), vec3_ti(mid1).toVec3t().add(1), .4f, true);
	} else {
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(mid1).toVec3t(), vec3_ti(mid1).toVec3t().add(1), .4f, true);
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(bottom2).toVec3t(), vec3_ti(bottom2).toVec3t().add(1), .4f, true);
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(bottom1).toVec3t(), vec3_ti(bottom1).toVec3t().add(1), .4f, true);
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(bottom3).toVec3t(), vec3_ti(bottom3).toVec3t().add(1), .4f, true);
		DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(vec3_ti(bottom4).toVec3t(), vec3_ti(bottom4).toVec3t().add(1), .4f, true);
	}
}

void FuckHead::onDisable() {
}
