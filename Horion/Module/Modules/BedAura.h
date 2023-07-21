#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class BedAura : public IModule {
private:
	int placeDelay;  // �����ӳ�
	int clickDelay;  // ����ӳ�

	int clickTimer = 0;

	//bool isSideEmpty();  // �ж�Ŀ����Χ�Ƿ�Ϊ��

public:
	int range;
	BedAura();
	~BedAura();
	// ���ģ����Ϣ
	const char* getModuleName() override;



	// ģ�鹦��
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;

	// ��Ϸ�߼�
	virtual void onTick(C_GameMode* gm) override;
};
