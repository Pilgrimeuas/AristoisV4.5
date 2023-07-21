#pragma once
#include "Module.h"

class AutoTotem : public IModule {
public:
	bool totem = true;

	AutoTotem();
	~AutoTotem();

	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
};