#pragma once
#include "Module.h"
class AutoText : public IModule {
private:
	bool hasClicked = false;
	bool MidClick = false;


public:
	SettingEnum mode;
	std::string textstring = "/hub";
	std::string& getClientName() { return textstring; };
	AutoText();
	~AutoText();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
