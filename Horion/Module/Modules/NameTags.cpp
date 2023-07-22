#include "NameTags.h"

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

NameTags::NameTags() : IModule(0, Category::VISUAL, "shows who tf is joe") {
	registerBoolSetting("Underline", &underline, underline);
	registerBoolSetting("Armor", &displayArmor, displayArmor);
	registerBoolSetting("Health", &drawhealth, drawhealth);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
}

NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNameTags(C_Entity* ent, bool) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (ent != localPlayer) {
		if (ent->timeSinceDeath > 0)
			return;
		if (ent->getNameTag()->getTextLength() < 1)
			return;
		if (Target::isValidTarget(ent,false) && nameTagsMod != nullptr) {
			nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
			float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
			DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist), nameTagsMod->drawhealth);
			DrawUtils::flush();
		}
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys()) return;

	if (ingameNametagSetting)
		if (!gotPrevSetting) {
			lastSetting = *ingameNametagSetting;
			gotPrevSetting = true;
			*ingameNametagSetting = false;
		} else
			*ingameNametagSetting = false;  // disable other ppl's nametags

	g_Data.forEachEntity(drawNameTags);
}

void NameTags::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		if (ingameNametagSetting && gotPrevSetting) {
			*ingameNametagSetting = lastSetting;
			gotPrevSetting = false;
		}
		//*ingameNametagSetting = true; //fix crash
	}
}