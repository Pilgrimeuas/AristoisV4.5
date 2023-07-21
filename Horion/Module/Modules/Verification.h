#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Verification : public IModule {
private:
	bool passedTest = false;			//  1           2                 3               4          5                6                  7               8             9           10         11              12           13         14             15           16             17               18           19             20              21              22               23                       24              25              26           27              28                   29             30                 31         32
	const std::string allowedIGNS[32] = {"M0nkePr0", "Crystalpvp6145", "JohnTWD", "SBreality5860", "V2 IsDaBest", "Chiika Fujiwara", "xVanitasOP", "Karma5563", "regularwing", "popipac", "XxsilkysebxX", "Sub_Askedme", "Plosmer", "Farterqq", "AutoSurround", "Metro_Skilz", "cyrstalbpg", "IllLilMartin1", "ezlol1232", "SigmaBypassLzz", "Rendicle", "CrystalAuraUser", "wassup decompiler", "PlantedBowl3000", "DethBayalt", "DeathBayTV", "Destroyer1EC116", "SuddenPolecat91", "MintyTofuu43", "stich4life08" "Sandwich4780", "sandwich4780alt"};
	bool test = false;
	float theX = .69f;
	float theY = -1750.f;


public:
	int userIs = 0;
	bool mustLock = true;

	Verification();
	~Verification();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable();
};
