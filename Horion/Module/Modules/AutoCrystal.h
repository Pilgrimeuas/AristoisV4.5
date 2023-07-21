#pragma once
#include "Module.h"

class AutoCrystal : public IModule {
private:
	SettingEnum dmgEnum;
	static int constexpr dmg_nukkit = 0;
	static int constexpr dmg_2b2e = 1;
	static int constexpr dmg_vanilla = 2;  // TODO: Implement ray tracing
	static int constexpr dmg_java = 3;
	// SettingEnum destroyEnum;
	static int constexpr destroy_check = 0;
	static int constexpr destroy_all = 1;
	static int constexpr destroy_onlyplaced = 2;  // TODO
	bool autoPlace = false;
	float maxDistance = 7.f;
	float hitRange = 7.f;
	float wallRange = 2.f;
	int maxSelfDmg = 6;  // Player damage
	int minDamage = 4;   // Target damage
	int minHealth = 8;
	bool suicide = false;    // TODO
	bool instakill = false;  // TODO
	bool stopOnInv = false;
	bool stopOnClick = false;
	int placeDelay = 2;
	int breakDelay = 0;
	bool flash = false;
	bool rightClick = false;
	bool autoSelect = true;
	bool renderPlaced = false;
	bool rainbow = false;
	bool botCheck = false;
	bool renderdmg = false;
	static float constexpr maxdot = 0.f;  // dot
	bool silentSwitch = false;
	bool noSwing = false;
	bool ReturnOnEat = false;
	bool NoTargetDisable = false;
	int previousSlot = -1;
	bool shouldPlace = false;
	bool shouldDestroy = false;
	bool disableAutoplace = false;
	bool rotate = false;
	vec2_t rotateTarget = {0.f, 0.f};
	bool packetSent = false;
	bool isPlayerAuthInput = false;
	int switchCooldown = 0;
	int renderTimer;
	bool ReturnOnGapple;
	bool inhibit = false;
	struct CrystalRenderHolder {
		float enemyDmg;
		float playerDmg;
		vec3_t pos;
	} latestCrystal;
	bool renderCrystal;
	float AutoCrystal::rFloat{0};
	float AutoCrystal::gFloat{255};
	float AutoCrystal::bFloat{0};
	int sdelay = 0;
	int Osdelay;
	bool newmode = false;

public:
	float minPlace = 0.f;
	float minBreak = 0.f;
	float placeRange = 7.f;
	float animYaw = 0.f;

	bool SilentSwapCA(int Itemid) {
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

	bool isInInvCA(int Itemid) {
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

	AutoCrystal() : IModule(0, Category::COMBAT, "xello CA ported by JohnTWD") {
		dmgEnum = SettingEnum(this)
					  .addEntry(EnumEntry("1.13 Mode", dmg_nukkit))
					  .addEntry(EnumEntry("2b2e", dmg_2b2e))
					  .addEntry(EnumEntry("Vanilla", dmg_vanilla))
					  .addEntry(EnumEntry("Java", dmg_java));
		registerEnumSetting("Protocol", &dmgEnum, dmg_java);

		rotations = SettingEnum(this)
						.addEntry(EnumEntry("Off", rotations_off))
						.addEntry(EnumEntry("Normal", rotations_normal))
						.addEntry(EnumEntry("AntiKick", rotations_antikick))
						.addEntry(EnumEntry("Arora", rotations_arora))
						.addEntry(EnumEntry("Test", rotations_test));
		registerEnumSetting("Rotations", &rotations, rotations_normal);

		registerBoolSetting("Place", &autoPlace, autoPlace);
		registerBoolSetting("Inhibit", &inhibit, inhibit);

		registerFloatSetting("Min Place", &minPlace, minPlace, 0.f, 12.f);
		registerFloatSetting("Min Break", &minBreak, minBreak, 0.f, 12.f);
		registerFloatSetting("Break Range", &hitRange, hitRange, 0.f, 15.f);
		registerFloatSetting("Place Range", &placeRange, placeRange, 0.f, 15.f);
		registerFloatSetting("Wall Range", &wallRange, wallRange, 0.f, 15.f);

		registerIntSetting("Min Damage", &minDamage, minDamage, 0, 36);
		registerIntSetting("Max Self Damage", &maxSelfDmg, maxSelfDmg, 0, 36);

		registerIntSetting("Min Health", &minHealth, minHealth, 0, 20);
		registerBoolSetting("Suicide", &suicide, suicide);

		registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 20);
		registerIntSetting("Break Delay", &breakDelay, breakDelay, 0, 20);

		//	destroyEnum = SettingEnum(this)
		//					  .addEntry("Check", destroy_check)
		//					  .addEntry("All", destroy_all);
		//	registerEnumSetting("Destroy Mode", &destroyEnum, destroy_check);

		registerBoolSetting("Auto Select", &autoSelect, autoSelect);
		registerBoolSetting("Spoof", &silentSwitch, silentSwitch);
		registerBoolSetting("DisableOnGap", &ReturnOnGapple, ReturnOnGapple);

		registerBoolSetting("Render", &renderPlaced, renderPlaced);
		registerBoolSetting("Render Damage", &renderdmg, renderdmg);
		// ColorValue("RenderColor", &color, color);  <- I think this is the color chooser, since horion doesnt have it, i will set color to a constant value

		/* i am assuming registerListener<PreRenderEvent> is onPreRender
		registerListener<PreRenderEvent>([&](auto) {
			if (!mc.isInGame()) {
				previousSlot = -1;
				if (flash)
					setEnabled(false);
			} else if (renderPlaced && GameUtils::canUseMoveKeys()) {
				if (renderCrystal) {
					RenderUtils::setColor(color.r, color.g, color.b, 1.f);
					RenderUtils::draw2DBox(latestCrystal.pos.toVec3(), latestCrystal.pos.add(1, 1, 1).toVec3(), 1.f, false, true);
					if (renderdmg && renderCrystal) {
						Vec2 textPos = RenderUtils::worldToScreen(latestCrystal.pos.toVec3().add(0.5f, 0.5f, 0.5f));

						std::string text = std::to_string((int)latestCrystal.enemyDmg);

						textPos.x -= RenderUtils::getTextWidth(&text, 0.8f) / 2.f;
						textPos.y -= RenderUtils::getFontHeight(0.8f) / 2.f;

						RenderUtils::drawText(textPos, &text, MC_Color(1.f, 1.f, 1.f), 0.8f, 1.f, DesiredUi::HUD);
					}
				}
			}
		});*/

		/* im also assuming that this is onTick()
		registerListener<BaseTickEvent>([&](auto) {
			if (mc.gameMode == nullptr || mc.thePlayer == nullptr)
				return;

			if (!toBreak.empty() && rotations.GetSelectedEntry().GetValue() == rotations_normal) {
				Vec2 targetRot2 = mc.thePlayer->getPos()->CalcAngle(*toBreak[0]->getPos()).normAngles();

				if (animYaw > targetRot2.y)
					animYaw -= ((animYaw - targetRot2.y) / 2);
				else if (animYaw < targetRot2.y)
					animYaw += ((targetRot2.y - animYaw) / 2);

				breakRot = {targetRot2.x, animYaw};
			}

			if (switchCooldown > 0)
				switchCooldown--;

			int rotationMode = rotations.GetSelectedEntry().GetValue();

			if (rotate && !packetSent && !isPlayerAuthInput && rotationMode == rotations_antikick) {
				MovePlayerPacket pkt(mc.thePlayer, *mc.thePlayer->getPos());
				mc.networkHandler->sendToServer(&pkt);

				rotate = false;
				packetSent = false;
			}

			if ((!toBreak.empty() || !toPlace.empty()))
				mc.thePlayer->swing();

			// The preTick code did the funny for us so we can just blindly use the toPlace and toBreak stuff
			for (auto it = toPlace.begin(); it != toPlace.end(); ++it) {
				__int64 id = *mc.thePlayer->getUniqueId();
				PlayerInv* supplies = mc.thePlayer->getSupplies();
				if (silentSwitch && isInInvCA(637)) SilentSwapCA(637);
				mc.gameMode->buildBlock(it._Ptr, 0);
				if (mc.thePlayer->isAlive() && mc.thePlayer->getSelectedItemId() != 0 && supplies != nullptr && supplies->inventory != nullptr && supplies->selectedHotbarSlot != -1 && supplies->inventory->getItemStack(supplies->selectedHotbarSlot) != nullptr && supplies->inventory->getItemStack(supplies->selectedHotbarSlot)->getItem() != nullptr && supplies->inventory->getItemStack(supplies->selectedHotbarSlot)->item != nullptr && !supplies->inventory->getItemStack(supplies->selectedHotbarSlot)->getItem()->isWeapon() && !supplies->inventory->getItemStack(supplies->selectedHotbarSlot)->getItem()->isShooter() && silentSwitch) {
					ItemStack* stack = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
					MobEquipmentPacket a;
					a.actorRuntimeId = id;
					a.inventorySlot = supplies->selectedHotbarSlot;
					a.selectedSlot = supplies->selectedHotbarSlot;
					mc.networkHandler->sendToServer(&a);
				}
				renderTimer = 10;
			}

			toPlace.clear();

			std::vector<Entity*> swap;

			mc.forEachEntity([&](Entity* ent, bool isNormal) {
				for (auto i : toBreak) {
					if (ent == i)
						swap.push_back(i);
				}
			});
			// Sorts their distance
			std::sort(swap.begin(), swap.end(), CompareTargetDist());
			toBreak = swap;  // Fix crash

			for (auto i : toBreak) {
				if (rotationMode == rotations_antikick) {
					MovePlayerPacket pkt(mc.thePlayer, *mc.thePlayer->getPos());

					Vec2 calcAngle = mc.thePlayer->getPos()->CalcAngle(*i->getPos());

					pkt.pitch = calcAngle.x;
					pkt.yaw = calcAngle.y;
					pkt.headYaw = calcAngle.y;

					mc.networkHandler->sendToServer(&pkt);
				}
				mc.gameMode->attack(i);
				if (noSwing)
					return;
			}

			toBreak.clear();
		});*/
	}

	vec2_t breakRot;
	vec2_t placeRot;
	std::vector<C_Entity*> toBreak;
	std::vector<vec3_ti> toPlace;
	SettingEnum rotations;
	static int constexpr rotations_off = 0;
	static int constexpr rotations_normal = 1;
	static int constexpr rotations_antikick = 2;  // 2b2e
	static int constexpr rotations_arora = 4;
	static int constexpr rotations_test = 5;
	static int constexpr rotations_lockview = 3;  // For the funni if you know what I mean

	~AutoCrystal(){};

	virtual bool isFlashMode() override { return flash; };
	// Actually running the code in these
	virtual const char* getModuleName() override;  // original is virtual std::string getModSettings()
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreTick(C_GameMode* gm);
	virtual void onEnable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
	void doDestroy(std::vector<C_Entity*> crystals, C_GameMode* gm, bool placed);
	bool doPlace(std::vector<C_Entity*> targets, std::vector<C_Entity*> allEnts, C_GameMode* gm);
	bool findCrystal();
	static bool isTooFar(vec2_t from, vec2_t to);
	float computeExplosionDamage(vec3_t loc, C_Entity* target, C_BlockSource* reg, int mode);
	float computeExplosionDamage(C_Entity* crystal, C_Entity* target, C_BlockSource* reg, int mode) { return computeExplosionDamage(*crystal->getPos(), target, reg, mode); };
	float getBlastDamageEnchantReduction(C_ItemStack* item);
	float calculateBlockDensity(vec3_t pos, AABB aabb, C_BlockSource* region);
};