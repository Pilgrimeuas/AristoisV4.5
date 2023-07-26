#include "ESP.h"
#include "../../../SDK/CClientInstance.h"
#include "../../../Utils/Target.h"

ESP::ESP() : IModule(0x0, Category::VISUAL, "Makes it easier to find entities around you") {
	registerIntSetting("Player R", &this->plrR, this->plrR, 0, 255);
	registerIntSetting("Player G", &this->plrG, this->plrG, 0, 255);
	registerIntSetting("Range", &this->plrB, this->plrB, 5, 255);
	registerIntSetting("Mob R", &this->mobR, this->mobR, 0, 255);
	registerIntSetting("Mob G", &this->mobG, this->mobG, 0, 255);
	registerIntSetting("Mob B", &this->mobB, this->mobB, 0, 255);
	registerBoolSetting("MobESP", &this->isMobEsp, this->isMobEsp);
	registerBoolSetting("ItemESP", &this->isItemESP, this->isItemESP);
	registerBoolSetting("2d", &this->is2d, this->is2d);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

static std::vector<C_Entity*> coloredFolks;
bool coloredFolkFinder(C_Entity* curEnt, bool isRegularEntity) {
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

	coloredFolks.push_back(curEnt);
	return true;
}

static std::vector<C_Entity*> friendmanESP;
bool friendFinderESP(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt, false)) return false;
	if (FriendList::findPlayer(curEnt->getNameTag()->getText())) {
		friendmanESP.push_back(curEnt);
		return true;
	}
	return false;
}

static std::vector<C_Entity*> mobESP;
bool mobFinderESP(C_Entity* curEnt, bool isRegularEntity){
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 319) return false;                   // player
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org

	mobESP.push_back(curEnt);
	return true;
};

static std::vector<C_Entity*> itemESP;
bool itemFinderESP(C_Entity* curEnt, bool isRegularEntity) {
	static auto espmod = moduleMgr->getModule<ESP>();
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (curEnt->getEntityTypeId() == 64) {
		float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());  // 更加曼妙的玩家定位
		if (dist < espmod->plrB) {
			itemESP.push_back(curEnt);  // 如果目标远处距离就返回
		}                               // item
			return true;
		
	} 
	return false;
};

//void ESP::onPlayerTick(C_Player* player) {
	/*
	if (g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())
		return;
	
	coloredFolks.clear();
	friendmanESP.clear();
	mobESP.clear();
	itemESP.clear();

	g_Data.forEachEntity(coloredFolkFinder);
	g_Data.forEachEntity(friendFinderESP);
	if (isMobEsp)
		g_Data.forEachEntity(mobFinderESP);
	if (isItemESP)
		g_Data.forEachEntity(itemFinderESP);*/


void ESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	static auto espMod = moduleMgr->getModule<ESP>();
	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		coloredFolks.clear();
		friendmanESP.clear();
		mobESP.clear();
		itemESP.clear();

		g_Data.forEachEntity(coloredFolkFinder);
		g_Data.forEachEntity(friendFinderESP);
		if (isMobEsp)
			g_Data.forEachEntity(mobFinderESP);
		if (isItemESP)
			g_Data.forEachEntity(itemFinderESP);


		for (C_Entity* ent : coloredFolks) {
			DrawUtils::setColor(plrR, plrG, plrB, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : friendmanESP) {
			DrawUtils::setColor(0, 255, 0, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : mobESP) {
			DrawUtils::setColor(mobR, mobG, mobB, 1.f);
			if (is2d) {
				DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));
			} else {
				DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()))));
			}
		}

		for (C_Entity* ent : itemESP) {
			if (ent->getEntityTypeId() == 64 && espMod->isItemESP) {
				C_ItemStack* stack = (C_ItemStack*)((uintptr_t)ent + 0x728);
				//std::string jb = stack->getItem()->name.getText();
				std::stringstream textbuild;
				if (stack->item != nullptr) {
					
					if (stack->count > 1)
						textbuild << std::to_string(stack->count) << "x ";

					bool wasSpace = true;
					std::string name = stack->getItem()->name.getText();
					for (auto i = 0; i < name.size(); i++) {
						if (wasSpace) {
							name[i] = toupper(name[i]);
							wasSpace = false;
						}

						if (name[i] == '_') {
							wasSpace = true;
							name[i] = ' ';
						}
					}

					textbuild << name;
				} else {
					textbuild << "Item";
				}
				vec3_t end = ent->getHumanPos().add(-.5f,-0.2,-.5f);
				std::string text = textbuild.str();
				vec2_t textPos = DrawUtils::worldToScreen(*ent->getPos());
				//float size = fmax(0.6f, 3.f / g_Data.getClientInstance()->levelRenderer->origin.dist(*ent->getPos()));

				//float itemHeight = 15.f * size;
				//float textHeight = DrawUtils::getFontHeight(size);
				//float itemSize = size;

			/*	if (itemHeight > textHeight) {
					itemSize *= textHeight / itemHeight;
					itemHeight = textHeight;
				}*/

				//float textWidth = DrawUtils::getTextWidth(&text, size) + (15.f * itemSize + 3.f * size);


				// if (!moduleMgr->getModule<NotoSans>()->isEnabled()) {
				// textPos.y += 1.f * size;
				// }

				// DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), 0.8f);

				//DrawUtils::drawItem(stack, itemPos, 1.f, itemSize, false);
				DrawUtils::drawTextInWorld(&text, end, 1, vec3_ti(255, 255, 255), vec3_ti(255, 255, 255), .0f);
				//DrawUtils::drawTextInWorld(&jb, end, 1, vec3_ti(66, 238, 238), vec3_ti(88, 88, 88), .1f);
				//DrawUtils::setColor(0, 0, 255, 1.f);
				//DrawUtils::drawEntityBox(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getHumanPos().dist(ent->getHumanPos()) * 3.f)));

				DrawUtils::flush();
			}
		}
	}
}
