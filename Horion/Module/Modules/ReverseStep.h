#pragma once
#include "Module.h"
class ReverseStep : public IModule {
private:
	float pullSpeed = -1.f;

public:
	int range = 5;
	bool oby = true;
	bool bedrock = true;
	ReverseStep();
	~ReverseStep();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
};