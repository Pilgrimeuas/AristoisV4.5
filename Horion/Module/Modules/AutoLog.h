#pragma once
#include"Module.h"
#include "../ModuleManager.h"

class AutoLog : public IModule {
public:
	bool CustomMsg = true;
	std::string custommessage;
	int health = 6;
	SettingEnum ByeByeMessage;
	bool message = true;
	AutoLog();
	~AutoLog();
	std::string name = "AutoLog";
	inline std::string& getCustomMessage() { return custommessage; };
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
