#include "BedAura.h"
#include "../../../Utils/Target.h"

BedAura::BedAura() : IModule(VK_NUMPAD0, Category::COMBAT, "BedAura for Minecraft") {
	// ע������ѡ��
	registerIntSetting("Range", &range, range, 4, 10);
	registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 10);
	registerIntSetting("Click Delay", &clickDelay, clickDelay, 0, 10);
}

BedAura::~BedAura() {}

const char* BedAura::getModuleName() {
	return "OldBedAura";
}
static std::vector<C_Entity*> targetList;

bool Findent666(C_Entity* curEnt, bool isRegularEntity) {
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
	if (dist <= moduleMgr->getModule<BedAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}
void BedAura::onEnable() {
	//targetList.clear();
}

void BedAura::onDisable() {
	targetList.clear();
	// ����ʱ�Ĵ��������ñ�����
}
bool spswap26(int Itemid) {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 36; n++) {
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
bool stopsp26() {
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
}
void BedAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	// ����Ⱦ֮ǰ��������Ʒ��ô�������
}

void BedAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown())
		return;
	// ��ȡ�������
	
	targetList.clear();
	g_Data.forEachEntity(Findent666);
	spswap26(418);
	if (!targetList.empty()) {
		for (auto& i : targetList) {
			vec3_t playerPos = (i->getHumanPos().floor());
			// �����ͷ�Ϸ��ô�
			playerPos.y += i->height + 1.f;  // ���ͷ��λ��
			vec3_t bedBlockPos = playerPos;
				gm->buildBlock(&vec3_ti(bedBlockPos), 0, true);  // ���ô�
			

			// �����
			if (clickDelay > 0) {
				
				if (clickTimer >= clickDelay) {
					g_Data.getCGameMode()->buildBlock(&vec3_ti(bedBlockPos), 0, true);  // �����
					clickTimer = 0;
				} else {
					clickTimer++;
				}
			}
		}
	}
	stopsp26();
	// ����Ŀ��ʵ��
	// ���������߼��ҵ�Ŀ��ʵ�壬�����丳ֵ��targetEntity����
}
