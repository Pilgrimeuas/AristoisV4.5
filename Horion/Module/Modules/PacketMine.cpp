#include "PacketMine.h"

#include "../../DrawUtils.h"

PacketMine::PacketMine() : IModule(0, Category::CPVP, "Mines blocks using magic") {
	registerIntSetting("Max range", &this->mineRange, this->mineRange, 1, 10);
}

PacketMine::~PacketMine() {
}

int PacketMine::ticksToMine(vec3_ti b) {
	return 0;
}

const char* PacketMine::getModuleName() {
	return "PacketMine";
}

vec3_ti block = vec3_ti(9999, -9999, 9999);  // empty value
bool startMine = false;

int blockFace = 2;

bool isValidBlock(vec3_ti block) {
	if (block == vec3_ti(9999, -9999, 9999))
		return false;

	int blockID = g_Data.getLocalPlayer()->region->getBlock(block)->toLegacy()->blockId;
	float dist = block.toVec3t().dist(g_Data.getLocalPlayer()->getHumanPos());

	if (dist > moduleMgr->getModule<PacketMine>()->mineRange) {
		startMine = false;
		return false;
	}

	if (blockID == 7 || blockID == 0) {  // bedrock/air
		if (blockID == 7)                // bedrock
			startMine = false;
		return false;
	}

	return true;
}
void falshPick() {  // get Piston 29
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 606 || stack->getItem()->itemId == 318 ) {  // select ¸ä×Ó
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool spoofgetv(int Itemid) {
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
bool stopspofv() {
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
bool isHoldingPickaxe() {
	int heldID = g_Data.getLocalPlayer()->getSelectedItemId();

	switch (heldID) {
	case 606:  // netherite
		return true;

	case 318:  // diamond
		return true;

	case 324:  // gold
		return true;

	case 297:  // iron
		return true;

	case 314:  // stone
		return true;

	case 310:  // wood
		return true;

	default:  // no pickaxe
		return false;
	}
}

void PacketMine::onEnable() {
	startMine = false;
}
void PacketMine::onLevelRender() {
	if (!g_Data.canUseMoveKeys() || !g_Data.isInGame() || g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
}

void PacketMine::onWorldTick(C_GameMode* gm) {
	if (!g_Data.canUseMoveKeys() || !g_Data.isInGame() || g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
}

void PacketMine::onTick(C_GameMode* gm) {
	if (!g_Data.canUseMoveKeys() || !g_Data.isInGame() || g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	if (g_Data.getLocalPlayer()->getlevel()->hasEntity())  // if theres an ent on our crosshair
		return;
	if (moduleMgr->getModule<CrystalPlace>()->isEnabled())
		return;
	if (moduleMgr->getModule<CevBreaker>()->isEnabled())
		return;
	if (moduleMgr->getModule<PistonAura>()->isEnabled())
		return;
	if (moduleMgr->getModule<AnchorAura>()->isEnabled())
		return;
	if (moduleMgr->getModule<AnvilAura>()->isEnabled())
		return;
	bool idk = false;
	if (g_Data.getLocalPlayer()->getlevel() != nullptr && g_Data.isLeftClickDown()) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;

		odelays = 0;

		block = g_Data.getLocalPlayer()->getlevel()->block;  // gets the block our crosshair is on
		blockFace = g_Data.getLocalPlayer()->getlevel()->blockSide;
		gm->startDestroyBlock(block, 0, idk);
		idk = 2;

		startMine = true;
	}

	if (!g_Data.isLeftClickDown()) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;

		bool idk = false;
		
		if (isValidBlock(block) && startMine) {
			odelays++;
			//gm->player->level->rayHitType = 3;
			//gm->player->level->rayHitVec = block.toVec3t();
			if (odelays >= 29) {
				
				falshPick();

				gm->destroyBlock(&block, blockFace);
				gm->stopDestroyBlock(block);
				//stopspofv();
				if (odelays >= 50) {
					odelays = 0;
					startMine = false;
					block = vec3_ti(9999, 100, 9999);
				}
			}
		}
		
	}
}

void PacketMine::onPreRender(C_MinecraftUIRenderContext* ctx) {
	//MC_Color arrayColor = DrawUtils::getWaveRGB(31, 240, 228, 0, 150, 255, 50 * 3);  // Wave
	//MC_Color rcolor = DrawUtils::getRainbowWave(0.5,255,200,-10);  // Wave
	float rentimer = 0;
	if (odelays != 0 && odelays <= 29) rentimer = (float)(odelays / 29.f);
	if (odelays >= 29) rentimer = 1;
	if (odelays >= 50) rentimer = 0;
	float zero = rentimer / 2;
	vec3_t pos(block.toVec3t().add(0.5));
	vec3_t pos2(block.toVec3t());
	//DrawUtils::setColor(.255f, .255f, .255f, 0.1f);
	{
		//DrawUtils::drawBox(pos2, pos2.add(1, 1, 1), 1, true);
	}

	DrawUtils::setColor(1.f, 1.f, 1.f, 0.2f);
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, -zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, -zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, -zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, -zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, zero, -zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(zero, -zero, -zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
	{
		vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, -zero));
		vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, zero, zero));
		vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, zero));
		vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(pos).add(-zero, -zero, -zero));
		DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
	}
}
