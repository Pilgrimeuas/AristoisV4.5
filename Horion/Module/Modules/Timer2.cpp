#include "Timer2.h"

Timer2::Timer2() : IModule(0, Category::MISC, "Velvet timer for test") {
	registerIntSetting("ticks per second", &timer, timer, 0, 500);
}

Timer2::~Timer2() {
}

const char* Timer2::getModuleName() {
	return ("Timer");
}

void Timer2::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Timer2::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}