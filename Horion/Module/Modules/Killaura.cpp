#include "Killaura.h"
Killaura::Killaura() : IModule('P', Category::COMBAT, "FK I DONT KNOW MAKE KA LOL") {
	rotations = SettingEnum(this)
					.addEntry(EnumEntry("None", 0))
					.addEntry(EnumEntry("Slient", 1))
					.addEntry(EnumEntry("Client", 2))
					.addEntry(EnumEntry("Actual", 3));
	registerEnumSetting("rotations", &rotations, 0);
	priority = SettingEnum(this)
				   .addEntry(EnumEntry("Distance", 0))
				   .addEntry(EnumEntry("Angle", 1))
				   .addEntry(EnumEntry("Health", 2))
				   .addEntry(EnumEntry("Armor", 3));
	registerEnumSetting("Priority", &priority, 0);
	registerEnumSetting("AutoWeapon", &autoweapon, 0);
	autoweapon.addEntry(EnumEntry("None", 0));
	autoweapon.addEntry(EnumEntry("Switch", 1));
	autoweapon.addEntry(EnumEntry("Silent", 2));
	registerBoolSetting("Timer", &timer1, timer1);        // 变速
	registerFloatSetting("tps", &timer, timer, 20, 100);  // 变速值
	// registerBoolSetting("Strafe", &strafe, strafe);
	registerBoolSetting("Swing", &swingKA, swingKA);
    registerBoolSetting("SingleAura", &isSingleAura, isSingleAura);
	registerBoolSetting("MulitAura", &isMulitAura, isMulitAura);
	//registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	// registerBoolSetting("Hurttime", &hurttime, hurttime);
	// registerBoolSetting("Silent Rotations", &silent, silent);
}
Killaura::~Killaura() {
}
const char* Killaura::getModuleName() {
	return ("Killaura");
}
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static std::vector<C_Entity*> targetList;
void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killaura = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;
	if (currentEntity->getEntityTypeId() == 64)  // item
		return;
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69) return;

	if (killaura->mobs) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319) return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) return;
		// if (currentEntity->getEntityTypeId() == 64) return;
		if (currentEntity->getEntityTypeId() == 0xC00050)  // Arrows - new item id
			return;
		if (currentEntity->getEntityTypeId() == 307)  // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity)) return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < killaura->range) {
		targetList.push_back(currentEntity);
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	}
}
void Killaura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

struct Distance {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*target->getPos()).dist(*localPlayer->getPos()) < (*target2->getPos()).dist(*localPlayer->getPos());
	}
};

struct Angle {
	bool operator()(C_Entity* target, C_Entity* target2) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		vec2_t angle = localPlayer->getPos()->CalcAngle(*target->getPos()).normAngles();
		vec2_t angle2 = localPlayer->getPos()->CalcAngle(*target2->getPos()).normAngles();
		vec2_t appl = localPlayer->viewAngles.normAngles();

		auto normAngles = [](float num) noexcept -> float {
			while (num > 180.0f)
				num -= 360.0f;
			while (num < -180.0f)
				num += 360.0f;
			return num;
		};

		// 当视角大于60度时，计算目标中点与准星中点的距离偏移，避免转头的pitch过于大。其他情况仅计算yaw的偏移

		return abs(g_Data.getLocalPlayer()->pitch) > 60.f ? abs(appl.sub(angle).normAngles().magnitude()) < abs(appl.sub(angle2).normAngles().magnitude()) : abs(normAngles(angle.y - appl.y)) < abs(normAngles(angle2.y - appl.y));
	}
};

struct Health {
	bool operator()(C_Entity* target, C_Entity* target2) {
		return target->getHealth() < target->getHealth();
	}
};

struct Armor {
	bool operator()(C_Entity* target, C_Entity* target2) {
		float armorValue = 0.f, armorValue2 = 0.f;

		for (int i = 0; i < 4; i++) {
			C_ItemStack* stack = target->getArmor(i);
			C_ItemStack* stack2 = target2->getArmor(i);

			if (stack->item != nullptr) {
				armorValue += stack->getArmorValueWithEnchants();
			}
			if (stack2->item != nullptr) {
				armorValue2 += stack2->getArmorValueWithEnchants();
			}
		}
		return armorValue < armorValue2;
	}
};
void Killaura::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	int dbrr = g_Data.getLocalPlayer()->getSelectedItemId();
	// if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && GameData::isRightClickDown()) return;  //吃东西停止杀戮光环
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 426 && g_Data.isRightClickDown())
		return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	Odelay++;
	slotKA = supplies->selectedHotbarSlot;
	targetListEmpty = targetList.empty();
	// Loop through all our players and retrieve their information
	targetList.clear();
	if (timer1) {
		g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
	}  // 变速齿轮
	g_Data.forEachEntity(findEntity);
	if (strafe) {
		localPlayer->setRot(angle);
	}
	if (autoweapon.selected == 2 && !targetList.empty())
		slot = supplies->selectedHotbarSlot;
	switch (priority.selected) {
	case 0:
		sort(targetList.begin(), targetList.end(), Distance());
		break;
	case 1:
		sort(targetList.begin(), targetList.end(), Angle());
		break;
	case 2:
		sort(targetList.begin(), targetList.end(), Health());
		break;
	case 3:
		sort(targetList.begin(), targetList.end(), Armor());
	}
	Odelay++;
		if (autoweapon.getSelectedValue() != 0 && !targetList.empty()) findWeapon();
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (!targetList.empty() && Odelay >= delay) {
		if (isSingleAura) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					if (swingKA) {
						player->swing();
					}
				}
			}
		}
		if (isMulitAura) {
			for (auto& i2 : targetList) {
				if (!(i2->damageTime > 1 && hurttime)) {
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					if (swingKA) {
						player->swing();
					}
				}
			}
		}
		if (autoweapon.getSelectedValue() == 2) supplies->selectedHotbarSlot = slotKA;
		if (rotations.getSelectedValue() == 3) {
			tro = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
			Odelay = 0;
	} else
		targetListEmpty = true;
	// gm->destroyBlock(&vec3_ti(277070, 69, -254355), 1);
}
void Killaura::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;

	if (!targetList.empty()) {

		for (auto& i : targetList) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			vec2_t pos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());

			if (rotations.getSelectedValue() == 2) {
				plr->yawUnused1 = angle.y;
				plr->pitch = angle.x;
				plr->bodyYaw = angle.y;
			}

			// Strafe
			if (rotations.getSelectedValue() == 3) {
				plr->setRot(tro);
				plr->yawUnused1 = angle.y;
			}
		}
	}
}
void Killaura::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListEmpty = true;
	targetList.clear();
}
void Killaura::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetListEmpty = true;
	targetList.clear();
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}
void Killaura::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (!targetList.empty()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() && (rotations.getSelectedValue() == 1 || rotations.getSelectedValue() == 2) && !targetList.empty()) {
			vec2_t anglefd = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->eyePos0);
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);

			movePacket->pitch = anglefd.x;
			movePacket->headYaw = anglefd.y;
			movePacket->yaw = anglefd.y;
		}
	}
	/*
	if (packet->isInstanceOf<C_TextPacket>()) {
		auto* textPacket = reinterpret_cast<C_TextPacket*>(packet);
		textPacket->message = textPacket->message.getText() + std::string("| Test");
	}
	*/
}