#pragma once
#include "Module.h"

class Watermark : public IModule {
public:
	Watermark();
	~Watermark();

	bool outline = false;
	bool background = false;
	bool top = true;
	bool bottom = false;
	bool rgba = true;


	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};