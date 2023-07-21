#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
class Compass2 : public IModule {
private:
	float opacity = 0.5;
	int range = 90;
	bool showWaypoints = true;
	void drawCenteredText(vec2_t pos, std::string text, float size, float textOpacity = 1);

public:
	Compass2();
	~Compass2();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
