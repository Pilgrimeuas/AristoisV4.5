#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit is a critical hit") {
}

Criticals::~Criticals() {
}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

void Criticals::onAttack(C_Entity* attackEnt) {
	static int a = 0;
	++a;
	if (a == 2) {
		a = 0;
		return;
	}
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	vec3_t localPlayerPos = *g_Data.getLocalPlayer()->getPos();
	C_PlayerActionPacket actionPacket;
	actionPacket.action = 8;  //jump
	actionPacket.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

	C_MovePlayerPacket movePacket;
	movePacket.onGround = false;
	movePacket.Position = {localPlayerPos.x, localPlayerPos.y + 1.2f, localPlayerPos.z};
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	movePacket.Position = {localPlayerPos.x, localPlayerPos.y + 0.4f, localPlayerPos.z};
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	C_Player* entity = reinterpret_cast<C_Player*>(attackEnt);
		entity->_crit(localPlayer);

	/*
	C_PlayerActionPacket actionPacket;
	actionPacket.action = 8; //jump
	actionPacket.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

	C_MovePlayerPacket movePacket;
	movePacket.onGround = false;
	movePacket.Position = *g_Data.getLocalPlayer()->getPos();
	movePacket.Position.y += 1.2f;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	Sleep(0.05);
	movePacket.Position.y -= 0.3f;
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
	//g_Data.getLocalPlayer()->setPos(g_Data.getLocalPlayer()->getPos()->add(0, 0.9, 0));
	*/
	}
