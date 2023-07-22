#include "RiptideFly.h"

RiptideFly::RiptideFly() : IModule(0, Category::MOVEMENT, "Fly like you have a trident") {
	registerFloatSetting("Horizontal Speed", &this->speed, this->speed, 0.1f, 2.f);
	registerFloatSetting("Vertical Speed", &this->upanddown, this->upanddown, 0.1f, 2.f);
	registerFloatSetting("value", &glideMod, glideMod, -0.01, -0.001);
	registerBoolSetting("AutoTrident", &trident, trident);
	registerBoolSetting("Rotations", &rotation, rotation);
}

RiptideFly::~RiptideFly() {
}

const char* RiptideFly::getModuleName() {
	return ("RiptideAura");
}
static std::vector<C_Entity*> ripList;
void FinRIP(C_Entity* currentEntity, bool isRegularEntity) {
	static auto Ripmode = moduleMgr->getModule<RiptideFly>();

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
	float dist = (currentEntity->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());  // 更加曼妙的玩家定位
	if (dist < 100) {
		ripList.push_back(currentEntity);  // 如果目标远处距离就返回
	}
}
void RiptideFly::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	// g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (trident) {
	
	for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 546) {  // select anchor
					supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}
	}
}
void RiptideFly::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	ripList.clear();
	g_Data.forEachEntity(FinRIP);
	g_Data.getLocalPlayer()->startSpinAttack();

	gm->player->velocity = vec3_t(0, 0, 0);
	glideModEffective = glideMod;
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (g_Data.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += upanddown;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= upanddown;
	}
	gm->player->velocity.y = glideModEffective;
	if (rotation) {
		angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*ripList[0]->getPos());
	}
}

void RiptideFly::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	float yaw = player->yaw;

	bool pressed = input->forward || input->backward || input->right || input->left;

	if (input->forward && input->backward)
		return;

	if (input->right) {
		yaw += 90.f;
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
	}
	if (input->left) {
		yaw -= 90.f;
		if (input->forward)
			yaw += 45.f;
		else if (input->backward)
			yaw -= 45.f;
	}

	if (input->backward && !input->left && !input->right)
		yaw += 180.f;
	float calcYaw = (yaw + 90) * (PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (pressed) player->lerpMotion(moveVec);
}

void RiptideFly::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (trident) {
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 259) {  // select anchor
					supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}
	}
	g_Data.getLocalPlayer()->stopSpinAttack();
}