#include "Timer.h"
#include "../ModuleManager.h"

Timer::Timer() : IModule(0, Category::MISC, "Increase the ticks per second") {
	registerFloatSetting("ticks per second", &timer, timer, 0, 100);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer");
}

static std::vector<C_Entity*> timerList;
void TimerFinder(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51)  // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}
	float dist = (currentEntity->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());  // 更加曼妙的玩家定位
	if (dist < killauraMod->range) {
		timerList.push_back(currentEntity);  // 如果目标远处距离就返回

	}
}
void Timer::onTick(C_GameMode* gm) {
	timerList.clear();
	static auto killauraMod = moduleMgr->getModule<Killaura>();
	g_Data.forEachEntity(TimerFinder);
	//bool targetListEmpty = timerList.empty();
	// Loop through all our players and retrieve their information
	
	if (!timerList.empty() && killauraMod->isEnabled()) {
		if ((g_Data.getLocalPlayer()->getSelectedItemId() == 259 || g_Data.getLocalPlayer()->getSelectedItemId() == 426) && g_Data.isRightClickDown()) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
		} else
			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
	}
}

void Timer::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}