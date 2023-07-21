#pragma once
#pragma once
#include "Module.h"
class SelfTrap : public IModule {
private:
	bool ATRots = false;
	bool fullbody = false;
	bool niggerTurnCAOn = false;
	bool switch2obby = true;
	bool airplace = false;
	bool ec = false;
	bool sb = false;
	bool hold = false;
	// ground level  ~~~ funni initializations
	vec3_t upper;
	vec3_t b1;
	vec3_t b2;
	vec3_t b3;
	// top level
	vec3_t b4;
	// middle level  ~ last priority because it is the least effective
	vec3_t mid1;
	vec3_t mid2;
	vec3_t mid3;
	vec3_t mid4;
	/* GROUND   FEET     HEAD
	   0 0 N	0 0 N    N N N
	   0 0 0    0 0 0    N 0 N
	   0 0 0	0 0 0    0 N N
	*/

	std::vector<vec3_t> placements;

public:
	bool mustGoUp = false;
	SettingEnum mode;
	SelfTrap();
	~SelfTrap();

	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	bool isHoldMode();
	//bool getobSpoof();
	//bool getespoof();
	virtual void onTick(C_GameMode* gm) override;
};
