#include "Burrow.h"

#include "../ModuleManager.h"

Burrow::Burrow() : IModule(0, Category::WORLD, "Burrow Into Blocks") {
	enum1 = SettingEnum(this)
				.addEntry(EnumEntry("Obsidian", 1))
				.addEntry(EnumEntry("EndChest", 2))
				.addEntry(EnumEntry("BlackBrick", 3));
	registerEnumSetting("Mode", &enum1, 1);
	this->registerBoolSetting("Rotate", &this->center, this->center);
	this->registerBoolSetting("Spoof", &this->spoof, this->spoof);
}

Burrow::~Burrow() {
}
const char* Burrow::getModName() {
	if (enum1.selected == 1) {
		return " [Obsidian]";
	} else {
		return " [Enderchest]";
	}
}
const char* Burrow::getModuleName() {
	return ("Burrow");
}

bool Burrow::isFlashMode() {
	return false;
}

bool Burrow::Center() {
	g_Data.getLocalPlayer()->applyTurnDelta(&vec2_t((float)(rand() % (2 * -1 - 1) + (-1)), (float)(rand() % (2 * -1 - 1) + (-1))));
	return true;
}

bool Burrow::tryScaffold(vec3_t blockBelow) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return false;

	blockBelow = blockBelow.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}

		if (center) Center();
		bool idk = false;
		g_Data.getCGameMode()->buildBlock(&blok, i, idk);

		return true;
	}
	return false;
}
bool spoofgetsb(int Itemid) {
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
bool stopspofsb() {
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
void Burrow::onTick(C_GameMode* gm) {
	auto clickguiMods = moduleMgr->getModule<Burrow>();
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	if (enum1.selected == 1) {
		if (!spoof) {
			for (int n = 0; n < 9; n++) {
				C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 130) {
						g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
					}
				}
			}
		}
		if (spoof) {
			spoofgetsb(130);
		}
	}
	if (enum1.selected == 0) {
		if (!spoof) {
			for (int n = 0; n < 9; n++) {
				C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 49) {
						g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
					}
				}
			}
		}
		if (spoof) {
			spoofgetsb(49);
		}
	}

	if (enum1.selected == 2) {
		if (!spoof) {
			for (int n = 0; n < 9; n++) {
				C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 525) {
						g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = n;
					}
				}
			}
		}
		if (spoof) {
			spoofgetsb(525);
		}
	}

	int place = 1;

	vec3_t blockSide = g_Data.getLocalPlayer()->eyePos0;

	blockSide.y -= g_Data.getLocalPlayer()->height - 1;

	if (!tryScaffold(blockSide)) {
		blockSide.y -= 0;
		blockSide.x -= 0;
	}
	if (spoof)
		stopspofsb();
}
void Burrow::onEnable() {}

void Burrow::onDisable() {
}

void Burrow::onSendPacket(C_Packet* packet) {
	timer++;
	vec3_t* pos = g_Data.getLocalPlayer()->getPos();
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* InputPacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		if (timer = 1) {
			InputPacket->Position.y = static_cast<float>(pos->y + 2.41999998688698);
		}
				if (timer = 2) {
			InputPacket->Position.y = static_cast<float>(pos->y + 2.7531999805211997);
		}
		if (timer = 3) {
			InputPacket->Position.y = static_cast<float>(pos->y + 3.00133597911214);
		}
		if (timer = 4) {
			InputPacket->Position.y = static_cast<float>(pos->y + 2.76610926093821);
		}
		if (timer = 5) {
			InputPacket->Position.y = static_cast<float>(pos->y + 1.36610926093821);
		}
	} else if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		movePacket->Position.y = static_cast<float>(pos->y - 1.36300926093821);
	}
}