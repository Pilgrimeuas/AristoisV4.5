#include "TargetHud.h"

TargetHud::TargetHud() : IModule(0, Category::COMBAT, "No Information") {
	registerIntSetting("X", &x, x, 0, 1000);

	registerIntSetting("Y", &y, y, 0, 500);

	registerIntSetting("Range", &range, range, 0, 15);

}

TargetHud::~TargetHud() {
}

const char* TargetHud::getModuleName() {
	return ("TargetHud");
}

static std::vector<C_Entity*> targetList;

void findEntityTH(C_Entity* currentEntity1, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<TargetHud>();

	if (currentEntity1 == nullptr)
		return;

	if (currentEntity1 == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity1, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity1->isAlive())
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity1->getNameTag()->getTextLength() <= 1 && currentEntity1->getEntityTypeId() == 63)
			return;
		if (currentEntity1->width <= 0.01f || currentEntity1->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity1->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity1))
			return;
	}

	float dist = (*currentEntity1->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity1);
	}
}

/*struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};*/

void TargetHud::onTick(C_GameMode* gm) {
	// Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntityTH);

	Odelay++;
	targethud++;
// Attack all entitys in targetList
	/*	if (isMulti) {
		for (auto& i : targetList) {
			if (!(i->damageTime > 1 && hurttime)) {
				// g_Data.getLocalPlayer()->swing();
				// g_Data.getCGameMode()->attack(i);
				targethud++;
			} else
				targethud = 0;
		}
	}
	Odelay = 0;
}*/
}

void TargetHud::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

void TargetHud::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.isInGame() && targetList.size() == 1) {
		if (targethud > 1) {
			for (auto& i : targetList) {
				C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
				vec2_t windowSize = dat->windowSize;
				std::string text = targetList[0]->getNameTag()->getText();
				text = Utils::sanitize(text);

				vec3_t targetpos = *targetList[0]->getPos();
				int tx = targetpos.x;
				int ty = targetpos.y;
				int tz = targetpos.z;
				auto posstring = std::string("Pos: " + std::to_string(tx) + ", " + std::to_string(ty) + ", " + std::to_string(tz));

				MC_Color hpcolor = MC_Color(0, 255, 0);

				int health = targetList[0]->getHealth();
				int maxhealth = 20;
				float health1 = targetList[0]->getHealth();
				float maxhealth1 = 20;
				float hpbarwidth = (health1 / maxhealth1) * 100;

				std::string healthstring = std::to_string(health) + "/" + std::to_string(maxhealth);
				int healthwidth = DrawUtils::getTextWidth(&healthstring);
				float dist = (*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
				float dist1 = round(dist);

				std::string distancestr = "Distance: " + std::to_string((int)dist) + std::string(".") + std::to_string((int)(dist * 10) - ((int)dist * 10)) + "m";
				std::string realname = text;

				float diswidth = DrawUtils::getTextWidth(&distancestr);
				float namewidth = DrawUtils::getTextWidth(&realname);

				// HEALTH COLOR
				MC_Color green = MC_Color(0, 255, 0);
				MC_Color red = MC_Color(255, 0, 0);



				// GUI SIZEEEEE

				vec4_t guisize = (vec4_t(x, y, x + windowSize.x / 5.5f, y + windowSize.y / 6.5f));
				// BACKGROUND
				DrawUtils::fillRoundRectangle(vec4_t(guisize),
											  MC_Color(0.f, 0.f, 0.f,0.26f),true );
				// LINE
				/*DrawUtils::fillRectangle(vec4_t(x + windowSize.x / 6.5f, y + windowSize.y / 7.f,
												x + windowSize.x / 6.5f, y + windowSize.y / 7.f - 1.f),
										 MC_Color(93, 63, 211), 0.75f);*/
				// PLAYER NAME
				DrawUtils::drawText(vec2_t(x + (windowSize.x / 5.5f) / 2 - healthwidth / 2,
										   y + 28.25f),
									&realname,
									MC_Color(0.23f, 0.8f, 0.37f), 0.6f);
				// DISTANCE
				/*				DrawUtils::drawText(vec2_t(x + 2.f,
										   y + 30.f + 12.f),
									&posstring,
									MC_Color(1.f, 1.f, 1.f), 1.f);*/
				// HP
				DrawUtils::drawText(vec2_t(x + (windowSize.x / 5.5f) / 2 - healthwidth / 2,
										   y + 30.f + 8.f),
									&healthstring,
									MC_Color(1.f, 1.f, 1.f), 1.f);
				// HP BAR FR
				if (health1 <= 6) {
					DrawUtils::fillRoundRectangle(vec4_t(x + windowSize.x / 5.5f / 2 - windowSize.x / 6.f / 2,
													y + 30.f + 8.f,
													(x + windowSize.x / 5.5f / 2 + windowSize.x / 6.f / 2) * hpbarwidth / 100,
													y + 30.f + 8.f + 10.f),
											 MC_Color(0.23f, 0.8f, 0.37f), 0.6f);
				} else if (health1 <= 14 && health1 > 6) {
					DrawUtils::fillRoundRectangle(vec4_t(x + windowSize.x / 5.5f / 2 - windowSize.x / 6.f / 2,
													y + 30.f + 8.f,
													(x + windowSize.x / 5.5f / 2 + windowSize.x / 6.f / 2) * hpbarwidth / 100,
													y + 30.f + 8.f + 10.f),
											 MC_Color(0.23f, 0.8f, 0.37f), 0.6f);
				} else {
					DrawUtils::fillRoundRectangle(vec4_t(x + windowSize.x / 5.5f / 2 - windowSize.x / 6.f / 2,
													y + 30.f + 8.f,
													(x + windowSize.x / 5.5f / 2 + windowSize.x / 6.f / 2) * hpbarwidth / 100,
													y + 30.f + 8.f + 10.f),
											 MC_Color(0.23f, 0.8f, 0.37f), 0.6f);
				}
				
				/*DrawUtils::fillRectangle(vec4_t(x + windowSize.x/5.5f/2 - windowSize.x/6.f/2,
												y + 30.f + 8.f,
												(x + windowSize.x / 5.5f / 2 + windowSize.x / 6.f / 2) * hpbarwidth / 100,
												y + 30.f + 8.f + 10.f),
										 MC_Color(0.23f, 0.8f, 0.37f), 0.6f);*/
				DrawUtils::flush();
				if (targetList[0]->isAlive() && targetList[0]->isPlayer()) {
					vec2_t textPos;
					vec4_t rectPos;
					float scale = 1.f;
					float spacing = scale + 18.5f;
					auto* player = reinterpret_cast<C_Player*>(targetList[0]);
					float x1 = (x + windowSize.x / 5.5f)/8.f + 7*x/8.f;
					//x + (windowSize.x / 5.5f) / 2 - namewidth / 2,
					float y1 = y + 12.f;
					// ARMOR
					for (int i = 0; i < 4; i++) {
						C_ItemStack* stack = player->getArmor(i);

						if (stack->item != nullptr) {
							CompoundTag* stacktag = stack->tag;
							float durmax = stack->getItem()->getMaxDamage();
							float currentdur = stack->getItem()->getDamageValue(stacktag);
							float currentdur1 = durmax - currentdur;
							int durper = (currentdur1 / durmax) * 100;
							std::string durstr = std::to_string(durper);
							int durwidth = DrawUtils::getTextWidth(&durstr, 0.9f);

							float fraction1 = currentdur1 / durmax;


		

							DrawUtils::drawText(vec2_t(x1 + 8.f - durwidth / 2, y1 - 9.f), &durstr, green, 0.9f);  // DRAW DURABILIK
							DrawUtils::drawItem(stack, vec2_t(x1, y1), 1.f, scale, stack->isEnchanted());
							x1 += scale * spacing;
						}
					}
					// ITEM
					{
						C_PlayerInventoryProxy* supplies = player->getSupplies();
						C_ItemStack* stack = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);

						if (stack->item != nullptr) {
							CompoundTag* stacktag = stack->tag;
							float durmax = stack->getItem()->getMaxDamage();
							float currentdur = stack->getItem()->getDamageValue(stacktag);
							float currentdur1 = durmax - currentdur;
							int durper = (currentdur1 / durmax) * 100;
							std::string durstr = std::to_string(durper);
							int durwidth = DrawUtils::getTextWidth(&durstr);

							int count = stack->count;
							std::string countstr = std::to_string(count);
							int countwidth = DrawUtils::getTextWidth(&countstr, 0.9f);

							float fraction1 = currentdur1 / durmax;

						

							//Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor1.r, gamerColor1.g, gamerColor1.b);
							if (!durmax == 0) {
								DrawUtils::drawText(vec2_t(x1 + 8.f - durwidth / 2, y1 - 9.f), &durstr, green, 0.9f);
							} else {
								DrawUtils::drawText(vec2_t(x1 + 8.f - countwidth / 2, y1 - 9.f), &countstr, MC_Color(23, 243, 223), 0.9f);
							}
							DrawUtils::drawItem(stack, vec2_t(x1, y1), 1.f, scale, stack->isEnchanted());
						}
					}
				}
			}
		}
	}
}
void TargetHud::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TargetHud::onDisable() {
	targethud = 0;
}