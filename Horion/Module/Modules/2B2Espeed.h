#pragma once
#include "Module.h"

class _2b2espeed_ : public IModule {
private:
	float speed = 0.325f;
	bool ssp = false;
	float sneakspeed = 1.f;
	float speedreal = 0.5f;

public:
	_2b2espeed_();
	~_2b2espeed_();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
