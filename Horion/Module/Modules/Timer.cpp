#include "Timer.h"
#include "../ModuleManager.h"

Timer::Timer() : IModule(0, Category::MISC, "Increase the ticks per second") {
	registerFloatSetting("ticks per second", &timer, timer, 0, 500);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer");
}

void Timer::onTick(C_GameMode* gm) {
	auto plr = g_Data.getLocalPlayer();
	static auto killauraMod = moduleMgr->getModule<Killaura>();
	static auto RiptideFlymod = moduleMgr->getModule<RiptideFly>();
	if (killauraMod->isEnabled() || RiptideFlymod->isEnabled()) {
		if ((g_Data.getLocalPlayer()->getSelectedItemId() == 259 || g_Data.getLocalPlayer()->getSelectedItemId() == 426) && g_Data.isRightClickDown()) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
		} else
			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
	}
}

void Timer::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}