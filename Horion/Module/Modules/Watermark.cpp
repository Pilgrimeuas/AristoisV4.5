#include "Watermark.h"
#include "../../DrawUtils.h"

Watermark::Watermark() : IModule(0, Category::HUD, "Watermark of thee client") {
	registerBoolSetting("Outline", &this->outline, this->outline);
	registerBoolSetting("Background", &this->background, this->background);
	registerBoolSetting("Bottom", &this->bottom, this->bottom);
	registerBoolSetting("RGBA", &this->rgba, this->rgba);
}

Watermark::~Watermark() {
}

const char* Watermark::getModuleName() {
	return ("Watermark");
}

void Watermark::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) 
		return;
	std::string playername2 = player->getNameTag()->getText();
	std::string playername = " | " + playername2;
	MC_Color arrayColor = DrawUtils::getWaveRGB(31, 240, 228, 0, 150, 255, 5 * 3);  // Wave
	static float rcolors[4];
	static float disabledRcolors[4];
	{
		Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
		disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
		disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
		disabledRcolors[3] = 1;
	}
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	constexpr float nameTextSize = 1.5f;
	constexpr float versionTextSize = 0.7f;
	static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
	constexpr float borderPadding = 1;
	constexpr float margin = 5;

	static std::string name = "AristoisV3";
#ifdef _DEBUG
	static std::string version = "";
#elif defined _BETA
	static std::string version = "";
#else
	static std::string version = "";
#endif

	float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
	float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
	if (bottom) {
		vec4_t rect = vec4_t(
			0.f + margin - borderPadding,
			windowSize.y + margin - textHeight,
			0.f + margin + fullTextLength + borderPadding * 2,
			windowSize.y + margin);

		if (background) {
			DrawUtils::fillRectangle(rect, MC_Color(12, 12, 12), 0.01f);
		}

		if (outline) {
			DrawUtils::drawRectangle(rect, MC_Color(arrayColor), 1.f, 1.f);
		}

		if (rgba) {
			DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(arrayColor), nameTextSize);
			DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.y), &playername, MC_Color(arrayColor), nameTextSize);
		} else {
			DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(255, 255, 255), nameTextSize);
			DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.y), &playername, MC_Color(255, 255, 255), nameTextSize);
		}
	} else {
		vec4_t rect = vec4_t(
			0.f + margin - borderPadding,
			21.f + margin - textHeight,
			0.f + margin + fullTextLength + borderPadding * 2,
			21.f + margin);

		if (background) {
			DrawUtils::fillRectangle(rect, MC_Color(12, 12, 12), 0.2f);
		}

		if (outline) {
			DrawUtils::drawRectangle(rect, MC_Color(arrayColor), 1.f, 1.f);
		}
		if (rgba) {
			DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(arrayColor), nameTextSize);
			DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.y), &playername, MC_Color(arrayColor), nameTextSize);
		} else {
			DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(255, 255, 255), nameTextSize);
			DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.y), &playername, MC_Color(255, 255, 255), nameTextSize);
		}
	}
}