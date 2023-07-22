#pragma once
#include "Module.h"

class Watermark : public IModule {
public:
	Watermark();
	~Watermark();
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	bool outline = false;
	bool background = false;
	bool top = true;
	bool bottom = false;
	bool rgba = true;
	int wx =0;
	int wy = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};