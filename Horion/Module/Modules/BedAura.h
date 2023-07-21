#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class BedAura : public IModule {
private:
	int placeDelay;  // 放置延迟
	int clickDelay;  // 点击延迟

	int clickTimer = 0;

	//bool isSideEmpty();  // 判断目标周围是否为空

public:
	int range;
	BedAura();
	~BedAura();
	// 外挂模块信息
	const char* getModuleName() override;



	// 模块功能
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;

	// 游戏逻辑
	virtual void onTick(C_GameMode* gm) override;
};
