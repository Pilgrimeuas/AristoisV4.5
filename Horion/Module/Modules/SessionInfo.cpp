#include "SessionInfo.h"

#include <chrono>

using namespace std;
SessionInfo::SessionInfo() : IModule(0, Category::WORLD, "Displays the SessionInfo") {
	registerEnumSetting("Theme", &mode, 0);
	mode.addEntry(EnumEntry("Packet", 0));
	mode.addEntry(EnumEntry("Ten", 1));
	registerFloatSetting("X", &siX, siX, 0, 1000);
	registerFloatSetting("Y", &siY, siY, 0, 500);
	registerBoolSetting("Outline", &outline, outline);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	shouldHide = true;
}

const char* SessionInfo::getModuleName() {
	return ("SessionInfo");
}

void SessionInfo::onTick(C_GameMode* gm) {
	if (ticks < 1260)
		ticks += 1;
	else
		ticks = 0;

	if (ticks2 < 20)
		ticks2 += 1;
	else
		ticks2 = 0;

	if (ticks3 < 72000)
		ticks3 += 1;
	else
		ticks3 = 0;

	if (ticks == 1260) min += 1;
	if (ticks2 == 20) sec += 1;
	if (ticks3 == 72000) hour += 1;

	if (sec == 60) sec = 0;
	if (min == 60) min = 0;
}

void SessionInfo::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();

	if (g_Data.canUseMoveKeys() && !clickGUI->isEnabled()) {
		auto interfaceColor = ColorUtil::interfaceColor(1);
		string title = "Statistics";
		string title2 = string(BOLD) + "Statistics";

		aidsLen = DrawUtils::getTextWidth(&string("Games Played: 0          "));
		vec4_t rectPos = vec4_t(siX, siY, siX + aidsLen, siY + 55);
		vec4_t rectPos2 = vec4_t(20, 20, 30, 30);
		vec4_t linePos = vec4_t(rectPos.x + 5, rectPos.y + 10, rectPos.z - 5, rectPos.y + 10.5);
		string test = to_string(min) + "m " + to_string(sec) + "s";
		if (hour >= 1) test = to_string(hour) + "h " + test;
		string add = "";

		string strings = "Games Played: \n" + add + "Play Time: \n" + add + "Kills: \n";
		string game = to_string(games);
		string kill = to_string(kills);

		string gamesPlayed = string(GRAY) + "Games Played: " + string(RESET) + game;
		string playersKilled = string(GRAY) + "Kills: " + string(RESET) + kill;
		string PlayTime = string(GRAY) + "Play Time: " + string(RESET) + test;
		int kde = 0;
		if (kills > 0) kde = (double)kills / (double)dead;
		string kd = to_string(kde);
		string kdStr = "KD: ";
		string kdStr2 = string(GRAY) + "KD: " + string(RESET) + kd;

		switch (mode.getSelectedValue()) {
		case 0:  // Packet
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y + 44), &kdStr, MC_Color(255, 255, 255), 1, 1, Fonts::SMOOTH);
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y + 10), &title, MC_Color(255, 255, 255), 1.f, true);
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y + 15), &strings, MC_Color(255, 255, 255), 1, 1, Fonts::SMOOTH);
			DrawUtils::drawRightAlignedString(&game, vec4_t(rectPos.x, rectPos.y + 13, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&kill, vec4_t(rectPos.x, rectPos.y + 33, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&test, vec4_t(rectPos.x, rectPos.y + 23, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&kd, vec4_t(rectPos.x, rectPos.y + 43, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::fillRoundRectangle(rectPos, MC_Color(0, 0, 0, opacity), true);
			if (outline) DrawUtils::drawRoundRectangle(rectPos, !clickGUI->isEnabled() ? interfaceColor : MC_Color(255, 255, 255), true);
			DrawUtils::fillRectangleA(linePos, MC_Color(255, 255, 255, 255));
			break;
			break;
		case 1:  // Tenacity
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y + 44), &kdStr, MC_Color(255, 255, 255), 1, 1, Fonts::SMOOTH);
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y), &title, MC_Color(255, 255, 255), 1.f, true);
			DrawUtils::drawText(vec2_t(rectPos.x + 4, rectPos.y + 15), &strings, MC_Color(255, 255, 255), 1, 1, Fonts::SMOOTH);
			DrawUtils::drawRightAlignedString(&game, vec4_t(rectPos.x, rectPos.y + 13, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&kill, vec4_t(rectPos.x, rectPos.y + 33, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&test, vec4_t(rectPos.x, rectPos.y + 23, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::drawRightAlignedString(&kd, vec4_t(rectPos.x, rectPos.y + 43, rectPos.z, rectPos.w), MC_Color(255, 255, 255), true);
			DrawUtils::fillRoundRectangle(rectPos, MC_Color(0, 0, 0, opacity), true);
			if (outline) DrawUtils::drawRoundRectangle(rectPos, !clickGUI->isEnabled() ? interfaceColor : MC_Color(255, 255, 255), true);
			DrawUtils::fillRectangleA(linePos, MC_Color(255, 255, 255, 255));
			break;
		}
	}
}
