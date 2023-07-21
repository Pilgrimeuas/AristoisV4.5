#include "RiptideFly.h"

RiptideFly::RiptideFly() : IModule(0, Category::MOVEMENT, "Fly like you have a trident") {
	registerFloatSetting("Horizontal Speed", &this->speed, this->speed, 0.1f, 2.f);
	registerFloatSetting("Vertical Speed", &this->upanddown, this->upanddown, 0.1f, 2.f);
}

RiptideFly::~RiptideFly() {
}

const char* RiptideFly::getModuleName() {
	return ("RiptideFly");
}

void RiptideFly::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
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
	g_Data.getLocalPlayer()->stopSpinAttack();
}