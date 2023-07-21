#pragma once
#pragma once
#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class PistonAura : public IModule {
private:
	int pdelay = 3;
	int rdelay = 0;
	int cdelay = 0;
	int cbrange = 3;
	int breakdelay = 0;
	int trapdelay = 0;
	bool spoof = false;
	bool airplace = false;
	bool high = false;
	bool normal = true;
	bool bypass = true;
	bool n2 = true;
	bool w2 = false;
	bool e2 = false;
	bool s2 = false;
	bool hasbreak = false;
	bool hasDetonated = false;
	bool hasPlacedAnchor = false;
	bool hasCharged = false;
	bool hasbb = false;
	bool hastrap = false;

	//
	bool DhasPlacedAnchor = false;
	bool DhasCharged = false;
	bool DhasDetonated = false;
	bool Dhasbb = false;
	bool Dtrap = false;
	//
	int tickTimer = 0;
	bool takenAnchor = false;
	bool takenGS = false;
	bool takenRS = false;
	bool takentrap = false;
	bool anti = false;

	//
	int Option = 99;
	SettingEnum lookat = this;
	// redstone level  ~~~ funni initializations
	vec3_t w;
	vec3_t e;
	vec3_t s;
	vec3_t n;
	vec3_t rw;
	vec3_t re;
	vec3_t rs;
	vec3_t rn;
	// top level
	vec3_t neckBreaker;
	// piston level  ~ last priority because it is the least effective
	vec3_t w1;
	vec3_t e1;
	vec3_t s1;
	vec3_t n1;
	// crystal level
	vec3_t cw;
	vec3_t ce;
	vec3_t cs;
	vec3_t cn;

public:
	PistonAura();
	~PistonAura();

	int range = 4;
	int crange = 10;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	void DestroyC(C_Entity* ent, int crange);
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	void CPlaceSB(C_GameMode* gm, vec3_t* pos);
	// bool SilentSwap2(int Itemid);
	bool tryPistonAura(vec3_t PistonAura);
	virtual void onDisable() override;
	virtual void onLevelRender() override;
};