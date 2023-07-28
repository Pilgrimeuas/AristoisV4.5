#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class AutoHotbar : public IModule {
private:
	bool Swordhb = false;
	bool Pickaxehb = false;
	bool obsidianhb = false;
	bool anvilhb = false;
	bool crystalhb = false;
	bool gapplehb = false;
	int SwordSlot = 1;
	int PickaxeSlot = 2;
	int obsidianSlot = 3;
	int anvilSlot = 4;
	int crystalSlot = 5;
	int gappleSlot = 9;
	bool dasword = false;
	bool dapickaxe = false;
	bool daob = false;
	bool daanvil = false;
	bool dacry = false;
	bool dagapple = false;
	bool Nodelay = false;

public:
	std::string name = "AutoHotbar";
	// SettingEnum mode = this;

	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	AutoHotbar();
};
