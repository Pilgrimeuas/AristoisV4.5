﻿#include "DrawUtils.h"

#include "Module/ModuleManager.h"
#include <Windows.h>
#include "../Utils/Logger.h"
#include "../Utils/TimerUtil.h"
#include <glm/ext/matrix_transform.hpp>

struct MaterialPtr {
	char padding[0x138];
};

using tess_vertex_t = void(__fastcall*)(Tessellator* _this, float v1, float v2, float v3);
using meshHelper_renderImm_t = void(__fastcall*)(__int64, Tessellator* tesselator, MaterialPtr*);

C_MinecraftUIRenderContext* renderCtx;
C_GuiData* guiData;
__int64 screenContext2d;
__int64 game3dContext;
Tessellator* tesselator;
float* colorHolder;
std::shared_ptr<glmatrixf> refdef;
vec2_t fov;
vec2_t screenSize;
vec3_t origin;
float lerpT;
C_TexturePtr* texturePtr = nullptr;

static MaterialPtr* uiMaterial = nullptr;
static MaterialPtr* blendMaterial = nullptr;
static MaterialPtr* entityFlatStaticMaterial = nullptr;

tess_vertex_t tess_vertex;
meshHelper_renderImm_t meshHelper_renderImm;
//mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
//Tessellator__initializeFormat_t Tessellator__initializeFormat;

bool hasInitializedSigs = false;
void initializeSigs() {
	
	tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("40 57 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24"));
	meshHelper_renderImm = reinterpret_cast<meshHelper_renderImm_t>(FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 49 8B F0 48 8B DA"));
	//mce__VertexFormat__disableHalfFloats = reinterpret_cast<mce__VertexFormat__disableHalfFloats_t>(FindSignature("40 53 48 83 EC ?? 48 8B D9 C7 81 ?? ?? ?? ?? 00 00 00 00 C6 81 ?? ?? ?? ?? 00"));
	//Tessellator__initializeFormat = reinterpret_cast<Tessellator__initializeFormat_t>(FindSignature("48 89 74 24 ?? 57 48 83 EC 20 4C 8B 41 ?? 48 8B FA 4C 2B 41 ?? 48 8B F1 48 83 C1 08 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 F7 E8 48 D1 FA 48 8B C2 48 C1 E8 3F 48 03 D0 48 3B FA"));
	hasInitializedSigs = true;
}

void DrawUtils::setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* gui) {
	if (!hasInitializedSigs)
		initializeSigs();
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

	ElapsedMicroseconds.QuadPart *= 1000000;
	int ticksPerSecond = 20;
	if (g_Data.getClientInstance()->minecraft != nullptr && reinterpret_cast<__int64>(g_Data.getClientInstance()->minecraft) < 0xFFFFFFFFFFF00000)
		if (g_Data.getClientInstance()->minecraft->timer != nullptr)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
	if(ticksPerSecond < 1)
		ticksPerSecond = 1;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	screenContext2d = reinterpret_cast<__int64*>(renderCtx)[2];

	tesselator = *reinterpret_cast<Tessellator**>(screenContext2d + 0xB0);
	colorHolder = *reinterpret_cast<float**>(screenContext2d + 0x30);

	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = g_Data.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if (uiMaterial == nullptr) {
		// 2 Sigs, wanted one comes first
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		uiMaterial = reinterpret_cast<MaterialPtr*>(sigOffset + offset + 7);
	}
	if (entityFlatStaticMaterial == nullptr && g_Data.isInGame()) {
		entityFlatStaticMaterial = reinterpret_cast<MaterialPtr*>(g_Data.getClientInstance()->itemInHandRenderer->entityLineMaterial.materialPtr);
	}
	if (blendMaterial == nullptr && g_Data.isInGame()) {
		//blendMaterial = reinterpret_cast<MaterialPtr*>(new mce::MaterialPtr("fullscreen_cube_overlay_blend"));
	}
}

void DrawUtils::setColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

C_Font* DrawUtils::getFont(Fonts font) {
	static auto fontChangerModule = moduleMgr->getModule<FontChanger>();
	if (fontChangerModule->Fonts.selected == 0) {  // notosans
		return g_Data.getClientInstance()->minecraftGame->getTheGoodFontThankYou();
	}
		if (fontChangerModule->Fonts.selected == 1) {  // mojang
		return g_Data.getClientInstance()->minecraftGame->mcFont;
	}
	if (fontChangerModule->Fonts.selected == 2) {  // ten
		return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[6].font;
	}
	if (fontChangerModule->Fonts.selected == 3) {  // Unicode
		return g_Data.getClientInstance()->minecraftGame->fontRepository->fontList->fontEntries[3].font;
	}

	switch (font) {
	case Fonts::SMOOTH:
		return g_Data.getClientInstance()->minecraftGame->getTheGoodFontThankYou();
		break;
	case Fonts::UNICOD:
		return g_Data.getClientInstance()->minecraftGame->getTheBetterFontYes();
		break;
	case Fonts::RUNE:
		return g_Data.getClientInstance()->_getRuneFont();
		break;
	case Fonts::MCFONT:
		return g_Data.getClientInstance()->minecraftGame->mcFont;
		break;
	default:
		return g_Data.getClientInstance()->_getFont();
		break;
	}
}

Tessellator* DrawUtils::get3dTessellator() {
	auto myTess = *reinterpret_cast<Tessellator**>(game3dContext + 0xB0);
	return myTess;
}

__int64 DrawUtils::getScreenContext() {
	return game3dContext == 0 ? screenContext2d : game3dContext;
}

MatrixStack* DrawUtils::getMatrixStack() {
	return reinterpret_cast<MatrixStack*>(*reinterpret_cast<__int64*>(DrawUtils::getScreenContext() + 0x18i64) + 0x30i64);
}

float DrawUtils::getTextWidth(std::string* textStr, float textSize, Fonts font) {
	TextHolder text(*textStr);

	C_Font* fontPtr = getFont(font);

	float ret = renderCtx->getLineLength(fontPtr, &text, textSize);

	return ret;
}

float DrawUtils::getFontHeight(float textSize, Fonts font) {
	C_Font* fontPtr = getFont(font);

	float ret = fontPtr->getLineHeight() * textSize;

	return ret;
}

void DrawUtils::flush() {
	renderCtx->flushText(0);
}

void DrawUtils::drawTriangle(const vec2_t& p1, const vec2_t& p2, const vec2_t& p3) {
	
	DrawUtils::tess__begin(tesselator, 3, 3);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}
MC_Color DrawUtils::getAstolfoRainbow(int yOffset, int yTotal) {
	auto hudModule = moduleMgr->getModule<HudModule>();
	float speed = 3000;
	float hue = (float)(TimerUtils::getCurrentMs() % (int)speed) + ((yTotal - yOffset) * 9);
	while (hue > speed) {
		hue -= speed;
	}
	hue /= speed;
	if (hue > 0.5) {
		hue = 0.5F - (hue - 0.5f);
	}
	hue += 0.5f;
	float r, g, b = 0;
	Utils::ColorConvertHSVtoRGB(hue, 0.5, 1.f, r, g, b);
	return MC_Color(true, r * 255, g * 255, b * 255, 255);
}
MC_Color DrawUtils::getWaveRGB(int red1, int green1, int blue1, int red2, int green2, int blue2, long index) {
	double offset = ((TimerUtils::getCurrentMs() - index) / 7) / (double)120;

	double aids123 = ((TimerUtils::getCurrentMs() - index) % 1000 / 1000.000);
	int aids1234 = ((TimerUtils::getCurrentMs() - index) % 2000 / 2000.000) * 2;

	aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
	double inverse_percent = 1 - aids123;
	int redPart = (int)(red1 * inverse_percent + red2 * aids123);
	int greenPart = (int)(green1 * inverse_percent + green2 * aids123);
	int bluePart = (int)(blue1 * inverse_percent + blue2 * aids123);
	return MC_Color(redPart, greenPart, bluePart);
}  // Deq Helpyd
MC_Color DrawUtils::getRainbowWave(float speedInSeconds, float s, float v, long index) {
	auto hudModule = moduleMgr->getModule<HudModule>();
	float hue = ((TimerUtils::getCurrentMs() + index) % (int)(((int)speedInSeconds) * 1000)) / (float)(((int)speedInSeconds) * 1000);
	float r, g, b = 0;
	Utils::ColorConvertHSVtoRGB(hue, 1.f, v, r, g, b);
	return MC_Color(true, r * 255, g * 255, b * 255, 255);
}  // Deq Helpyd


void DrawUtils::drawQuad(const vec2_t& p1, const vec2_t& p2, const vec2_t& p3, const vec2_t& p4) {
	DrawUtils::tess__begin(tesselator, 1, 4);

	tess_vertex(tesselator, p1.x, p1.y, 0);
	tess_vertex(tesselator, p2.x, p2.y, 0);
	tess_vertex(tesselator, p3.x, p3.y, 0);
	tess_vertex(tesselator, p4.x, p4.y, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawLine(const vec2_t& start, const vec2_t& end, float lineWidth) {
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	DrawUtils::tess__begin(tesselator, 3, 6);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	meshHelper_renderImm(screenContext2d, tesselator, uiMaterial);
}

void DrawUtils::drawText(const vec2_t& pos, std::string* textStr, const MC_Color& color, float textSize, float alpha, Fonts font) {
	TextHolder text(*textStr);
	C_Font* fontPtr = getFont(font);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;


	float posF[4];  // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y - 1;
	posF[3] = pos.y + 1000;

	TextMeasureData textMeasure{};
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;

	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}
void DrawUtils::drawCenteredString(vec2_t pos, std::string* textStr, float textSize, MC_Color color, bool hasShadow) {
	DrawUtils::drawText(vec2_t(pos.x + DrawUtils::getTextWidth(textStr, textSize) / 2.F, pos.y - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.F), textStr, color, textSize, 1);
}

void DrawUtils::drawX(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool outline) {
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = 0;
	diff.z = 0;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw

	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}

		drawLine(lastVertex, curVertex, lineWidth);
		DrawUtils::drawQuad({lower.x, lower.x}, {lower.z, lower.z}, {lower.y, lower.y}, {lower.x, lower.x});
		lastVertex = curVertex;
	}
}

void DrawUtils::drawY(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool outline) {
	vec3_t diff;
	diff.x = 0;
	diff.y = upper.y - lower.y;
	diff.z = 0;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw

	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}

		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawZ(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool outline) {
	vec3_t diff;
	diff.x = 0;
	diff.y = 0;
	diff.z = upper.z - lower.z;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw

	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}

		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawBox(const vec3_t& lower, const vec3_t& upper, float lineWidth, bool outline) {
	
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int) screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x; 
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y; 
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z; 
				
				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw
	
	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}
		
		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawImage(std::string filePath, vec2_t& imagePos, vec2_t& imageDimension, vec2_t& idk, vec2_t& idk2, MC_Color flushColor) {
	texturePtr = new C_TexturePtr();
	C_FilePath file(filePath);
	renderCtx->getTexture(texturePtr, file);
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1");

	if (texturePtr != nullptr) {
		renderCtx->drawImage(texturePtr, imagePos, imageDimension, idk, idk2);
		renderCtx->flushImages(flushColor, flushImageAddr, flushString);
		delete texturePtr;
	}
}
void DrawUtils::drawSystemImage(std::string filePath, vec2_t& imagePos, vec2_t& imageDimension, vec2_t& idk, vec2_t& idk2, MC_Color flushColor) {
	texturePtr = new C_TexturePtr();
	C_FilePath imagepath = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\") + std::string(filePath);
	renderCtx->getTexture(texturePtr, imagepath);
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1");

	if (texturePtr != nullptr) {
		renderCtx->drawImage(texturePtr, imagePos, imageDimension, idk, idk2);
		renderCtx->flushImages(flushColor, flushImageAddr, flushString);
		delete texturePtr;
	}
}
void DrawUtils::drawTextInWorld(std::string* textToSay, const vec3_t& location, float tsize, vec3_ti tColor, vec3_ti bgColor, float opacity) {
	vec2_t textPos;
	vec4_t rectPos;
	
	float textWidth = getTextWidth(textToSay, tsize);
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * tsize;

	vec3_t actualLocation = location.add(0.5f);
	
	if (refdef->OWorldToScreen(origin, actualLocation, textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * tsize;
		rectPos.y = textPos.y - 1.f * tsize;
		rectPos.z = textPos.x + textWidth + 1.f * tsize;
		rectPos.w = textPos.y + textHeight + 2.f * tsize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * tsize;
		fillRectangle(rectPos, MC_Color(bgColor.x, bgColor.y, bgColor.z), opacity);

		drawText(textPos, textToSay, MC_Color(tColor.x, tColor.y, tColor.z), tsize);
	}
}

std::string CODMW6969(std::string STUFF, int digits) {
	bool afterDecimalPt = false;
	int dp = 0;
	std::string toOutput;

	for (int i = 0; i < STUFF.length(); ++i) {
		if (STUFF.at(i) == '.') {
			afterDecimalPt = true;
		}

		toOutput.append(std::string(1, STUFF.at(i)));

		if (afterDecimalPt) {
			dp++;

			if (dp > digits)
				break;
		}
	}

	return toOutput;
}

// the 2nd one with fill
void DrawUtils::drawBoxF(const vec3_t& lower, const vec3_t& upper) {
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t vertices[4];
	vertices[0] = vec3_t{lower.x, lower.y, lower.z};
	vertices[1] = vec3_t{lower.x, lower.y, lower.z + diff.z};
	vertices[2] = vec3_t{lower.x + diff.x, lower.y, lower.z + diff.z};
	vertices[3] = vec3_t{lower.x + diff.x, lower.y, lower.z};
	// Convert to screen coord
	std::vector<vec2_t> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(screen);
		}
	}
	if (screenCords.size() < 2)
		return;

	drawQuad(screenCords[0], screenCords[1], screenCords[2], screenCords[3]);
}
void DrawUtils::drawBox3dFilled(const vec3_t& lower, const vec3_t& upper, float scale, bool outline, bool onUi) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();
	vec3_t diff = upper.sub(lower);
	vec3_t newLower = lower.sub(origin);
	vec3_t vertices[8] = {
		{newLower.x, newLower.y, newLower.z},
		{newLower.x + diff.x, newLower.y, newLower.z},
		{newLower.x, newLower.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y, newLower.z + diff.z},

		{newLower.x, newLower.y + diff.y, newLower.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z},
		{newLower.x, newLower.y + diff.y, newLower.z + diff.z},
		{newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z}};
	// Scale vertices using glm
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	vec3_t newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);  // .add(0.4375f, 0.4375f, 0.4375f) is chest
	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = vec3_t{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
	}

	DrawUtils::tess__begin(myTess, 1);
	static int v[48] = {5, 7, 6, 4, 4, 6, 7, 5, 1, 3, 2, 0, 0, 2, 3, 1, 4, 5, 1, 0, 0, 1, 5, 4, 6, 7, 3, 2, 2, 3, 7, 6, 4, 6, 2, 0, 0, 2, 6, 4, 5, 7, 3, 1, 1, 3, 7, 5};
	for (int i = 0; i < 48; i++) tess_vertex(myTess, vertices[v[i]].x, vertices[v[i]].y, vertices[v[i]].z);
	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);

	if (!outline) return;
	DrawUtils::tess__begin(myTess, 4, 12);
#define line(m, n)                      \
	tess_vertex(myTess, m.x, m.y, m.z); \
	tess_vertex(myTess, n.x, n.y, n.z);

	// Top square
	line(vertices[4], vertices[5]);
	line(vertices[5], vertices[7]);
	line(vertices[7], vertices[6]);
	line(vertices[6], vertices[4]);

	// Bottom Square
	line(vertices[0], vertices[1]);
	line(vertices[1], vertices[3]);
	line(vertices[3], vertices[2]);
	line(vertices[2], vertices[0]);

	// Sides
	line(vertices[0], vertices[4]);
	line(vertices[1], vertices[5]);
	line(vertices[2], vertices[6]);
	line(vertices[3], vertices[7]);

#undef line
	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);
}
void DrawUtils::drawBox3dNew(const vec3_t& lower, const vec3_t& upper, float scale, bool onUi) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	Tessellator* myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 12);

	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	auto newLower = lower.sub(origin);

	vec3_t vertices[8];
	vertices[0] = vec3_t(newLower.x, newLower.y, newLower.z);
	vertices[1] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z);
	vertices[2] = vec3_t(newLower.x, newLower.y, newLower.z + diff.z);
	vertices[3] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

	vertices[4] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z);
	vertices[5] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
	vertices[6] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
	vertices[7] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);
	// Scale vertices using glm
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(scale), 0.f, glm::vec3(1.0f, 1.0f, 1.0f));
	vec3_t newLowerReal = newLower.add(0.5f, 0.5f, 0.5f);  // .add(0.4375f, 0.4375f, 0.4375f) is chest
	for (int i = 0; i < 8; i++) {
		glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertices[i].x - newLowerReal.x, vertices[i].y - newLowerReal.y, vertices[i].z - newLowerReal.z, 0.0f);
		vertices[i] = vec3_t{rotatedVertex.x + newLowerReal.x, rotatedVertex.y + newLowerReal.y, rotatedVertex.z + newLowerReal.z};
	}

#define line(m, n)                      \
	tess_vertex(myTess, m.x, m.y, m.z); \
	tess_vertex(myTess, n.x, n.y, n.z);

	// Top square
	line(vertices[4], vertices[5]);
	line(vertices[5], vertices[7]);
	line(vertices[7], vertices[6]);
	line(vertices[6], vertices[4]);

	// Bottom Square
	line(vertices[0], vertices[1]);
	line(vertices[1], vertices[3]);
	line(vertices[3], vertices[2]);
	line(vertices[2], vertices[0]);

	// Sides
	line(vertices[0], vertices[4]);
	line(vertices[1], vertices[5]);
	line(vertices[2], vertices[6]);
	line(vertices[3], vertices[7]);

#undef line

	meshHelper_renderImm(game3dContext, myTess, onUi ? uiMaterial : blendMaterial);
}
void DrawUtils::drawNameTags(C_Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	vec2_t textPos;
	vec4_t rectPos;
	std::string text = ent->getNameTag()->getText();
	text = Utils::sanitize(text);
	text = text.substr(0, text.find('\n'));

	float textWidth = getTextWidth(&text, textSize);
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * textSize;

	if (refdef->OWorldToScreen(origin, ent->eyePos0.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * textSize;
		rectPos.y = textPos.y - 1.f * textSize;
		rectPos.z = textPos.x + textWidth + 1.f * textSize;
		rectPos.w = textPos.y + textHeight + 2.f * textSize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 0.75f * textSize;
		auto nametagsMod = moduleMgr->getModule<NameTags>();
		// fillRectangleA(rectPos, MC_Color(0, 0, 0, nametagsMod->opacity));
		if (ent->damageTime > 1)
			fillRectangleA(rectPos, MC_Color(1.f, 0.f, 0.f, nametagsMod->opacity));
		else
			fillRectangleA(rectPos, MC_Color(0.f, 0.f, 0.f, nametagsMod->opacity));
		auto interfaceColor = ColorUtil::interfaceColor(1);
		if (nametagsMod->underline) {
			fillRectangle(subRectPos, MC_Color(interfaceColor), 1.f);
		}
		if (FriendList::findPlayer(ent->getNameTag()->getText()) && ent->getNameTag()->getTextLength() >= 1 && ent->getEntityTypeId() == 319) {
			drawText(textPos, &text, MC_Color(0, 255, 0), textSize, 1.f, Fonts::SMOOTH);
		} else
			drawText(textPos, &text, MC_Color(255, 255, 255), textSize, 1.f, Fonts::SMOOTH);
		// drawText(textPos, &text, MC_Color(255, 255, 255), textSize, 1.f, true);

		static auto nameTagsMod = moduleMgr->getModule<NameTags>();

		//armor
		if (ent->getEntityTypeId() == 319 && nameTagsMod->displayArmor) {  // animals dont hav amor :rage:
			auto* player = reinterpret_cast<C_Player*>(ent);
			float scale = textSize * 1.f;
			float spacing = scale + 18.f;
			float x = rectPos.x;
			float y = rectPos.y - 20.f * scale;
			float itemHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * scale;
			// armor
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4);
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);

					DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, .8f, false);
					DrawUtils::drawText(vec2_t(x, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 1.f, 1.f, Fonts::SMOOTH);
					//DrawUtils::drawCenteredString(vec2_t(x + textSize * 4.5f, y), &durastr, 1, MC_Color(gamerColor), true);
					x += scale * spacing;
				}
			}
			// item
			{
				C_ItemStack* stack = player->getSelectedItem();
				if (stack->item != nullptr) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4);
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
					if (!dura1 == 0) DrawUtils::drawText(vec2_t(rectPos.z - 1.f - 15.f * scale, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 1.f, 1.f, Fonts::SMOOTH);
					//if (!dura1 == 0) DrawUtils::drawCenteredString(vec2_t(rectPos.z - textSize * 4.5f, y), &durastr, 1, MC_Color(gamerColor), true);
					DrawUtils::drawItem(stack, vec2_t(rectPos.z + 20.f * scale, y), 1.f, scale, false);
				}
			}
		}
	}
}


void DrawUtils::drawEntityBox(C_Entity* ent, float lineWidth) {
	vec3_t end = ent->eyePos0;
	AABB render(end, ent->width, ent->height, end.y - ent->aabb.lower.y);
	render.upper.y += 0.1f;

	drawBox(render.lower, render.upper, lineWidth, true);
}

void DrawUtils::draw2D(C_Entity* ent, float lineWidth) {
	vec3_t base = ent->eyePos0;
	float ofs = (g_Data.getLocalPlayer()->yaw + 90.f) * (PI / 180);

	vec3_t corners[4];
	vec2_t corners2d[4];

	corners[0] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z - ent->width / 1.5f * cos(ofs));
	corners[1] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z + ent->width / 1.5f * cos(ofs));
	corners[2] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z - ent->width / 1.5f * cos(ofs));
	corners[3] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z + ent->width / 1.5f * cos(ofs));

	if (refdef->OWorldToScreen(origin, corners[0], corners2d[0], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[1], corners2d[1], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[2], corners2d[2], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[3], corners2d[3], fov, screenSize)) {
		//float length = (corners2d[1].x - corners2d[0].x) / 4.f;

		/*drawLine(corners2d[0], vec2_t(corners2d[0].x + length, corners2d[0].y), lineWidth);
		drawLine(corners2d[0], vec2_t(corners2d[0].x, corners2d[0].y + length), lineWidth);

		drawLine(vec2_t(corners2d[1].x - length, corners2d[1].y), corners2d[1], lineWidth);
		drawLine(corners2d[1], vec2_t(corners2d[1].x, corners2d[1].y + length), lineWidth);

		drawLine(vec2_t(corners2d[2].x, corners2d[2].y - length), corners2d[2], lineWidth);
		drawLine(corners2d[2], vec2_t(corners2d[2].x + length, corners2d[2].y), lineWidth);

		drawLine(vec2_t(corners2d[3].x, corners2d[3].y - length), corners2d[3], lineWidth);
		drawLine(vec2_t(corners2d[3].x - length, corners2d[3].y), corners2d[3], lineWidth);*/

		drawLine(corners2d[0], corners2d[1], lineWidth);
		drawLine(corners2d[0], corners2d[2], lineWidth);
		drawLine(corners2d[3], corners2d[1], lineWidth);
		drawLine(corners2d[3], corners2d[2], lineWidth);
	}
}

void DrawUtils::drawItem(C_ItemStack* item, const vec2_t& itemPos, float opacity, float scale, bool isEnchanted) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<C_ScreenContext*>(scnCtx);
	C_BaseActorRenderContext baseActorRenderCtx(screenCtx, g_Data.getClientInstance(), g_Data.getClientInstance()->minecraftGame);
	C_ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, 0, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
}

vec2_t DrawUtils::worldToScreen(const vec3_t& world) {
	vec2_t ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
void DrawUtils::drawLine3d(const vec3_t& start, const vec3_t& end) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 2);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	tess_vertex(myTess, start1.x, start1.y, start1.z);
	tess_vertex(myTess, end1.x, end1.y, end1.z);

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::drawBox3d(const vec3_t& lower, const vec3_t& upper) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 4, 12);

	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	auto newLower = lower.sub(origin);

	vec3_t vertices[8];
	vertices[0] = vec3_t(newLower.x, newLower.y, newLower.z);
	vertices[1] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z);
	vertices[2] = vec3_t(newLower.x, newLower.y, newLower.z + diff.z);
	vertices[3] = vec3_t(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

	vertices[4] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z);
	vertices[5] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
	vertices[6] = vec3_t(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
	vertices[7] = vec3_t(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);

	#define line(m, n) tess_vertex(myTess, m.x, m.y, m.z); \
		tess_vertex(myTess, n.x, n.y, n.z);
	
	#define li(m, n) line(vertices[m], vertices[n]);

	li(0, 1);
	li(1, 3);
	li(3, 2);
	li(2, 0);

	li(4, 5);
	li(5, 7);
	li(7, 6);
	li(6, 4);

	li(0, 4);
	li(1, 5);
	li(2, 6);
	li(3, 7);

	#undef li
	#undef line
	
	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::drawLineStrip3D(const std::vector<vec3_t>& points, int mode) {
	if (game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	/*
	 * 1: quads
	 * 2: triangle list
	 * 3: trianglestrip (6)
	 * 4: line list
	 * 5: line strip (7)
	 */

	auto myTess = DrawUtils::get3dTessellator();
	DrawUtils::tess__begin(myTess, mode, (int)points.size());
	for (const auto& p : points) {
		auto pD = p.sub(origin);
		tess_vertex(myTess, pD.x, pD.y, pD.z);
	}

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::fillRectangle(const vec4_t& pos, const MC_Color& col, float alpha) {
	DrawUtils::setColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}
void DrawUtils::fillRectangleA(vec4_t pos, MC_Color col) {
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}
void DrawUtils::drawRoundRectangle(vec4_t pos, const MC_Color col, bool rounder = false) {
	switch (rounder) {
	case 0:
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 2, pos.z - 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1, pos.z - 1, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y, pos.x, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y, pos.z + 1, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w, pos.z, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w, pos.x + 1, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 1, pos.z, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 1, pos.x + 1, pos.y), col);
		break;
	case 1:
		DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y, pos.x, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y, pos.z + 1, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.w + 2, pos.z - 2, pos.w + 3), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.y - 3, pos.z - 2, pos.y - 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 2, pos.x + 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 2.5, pos.x + 2, pos.y - 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - .5, pos.y - 1, pos.x + .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 2, pos.z, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 2, pos.y - 2.5, pos.z - 1, pos.y - 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - .5, pos.y - 1, pos.z + .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x - .5, pos.w, pos.x + .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w + 1, pos.x + 1, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1.5f, pos.x + 2, pos.w + 2.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - .5, pos.w, pos.z + .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w + 1, pos.z, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.z - 2, pos.w + 1.5f, pos.z - 1, pos.w + 2.5), col);
		break;
	}
}
void DrawUtils::drawRightAlignedString(std::string* textStr, vec4_t pos, MC_Color color, bool hasShadow) {
	TextHolder text(*textStr);
	std::string test = text.getText();
	float lenth2 = DrawUtils::getTextWidth(&test, 1) + 2;
	vec4_t rectPos = vec4_t(pos.x, pos.y, pos.z, pos.y);
	vec2_t textPos = vec2_t(rectPos.z - lenth2, rectPos.y + 2);
	DrawUtils::drawText(textPos, &test, color, 1, 1, Fonts::SMOOTH);
}

void DrawUtils::fillRoundRectangle(vec4_t pos, const MC_Color col, bool rounder = false) {
	switch (rounder) {
	case 0:
		DrawUtils::setColor(col.r, col.g, col.b, col.a);
		DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1, pos.z - 1, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w, pos.z - 1, pos.w + 1), col);
		break;
	case 1:
		DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y, pos.z, pos.w), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.w, pos.z - .5, pos.w + 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w + 1, pos.z - 1, pos.w + 1.5), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.w + 1.5, pos.z - 2, pos.w + 2), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + .5, pos.y - 1, pos.z - .5, pos.y), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1.5f, pos.z - 1, pos.y - 1), col);
		DrawUtils::fillRectangleA(vec4_t(pos.x + 2, pos.y - 2, pos.z - 2, pos.y - 1.5), col);
		break;
	}
}

/*void DrawUtils::fillRoundRectangleGradient(vec4_t pos, int quality) {
	auto interface = moduleMgr->getModule<Interface>();
	std::string test;
	float drawquality = quality;
	int count = (pos.z - pos.x) / drawquality;
	int ind = 0;
	pos.z = pos.x + drawquality;
	for (int i = 0; i < count; i++) {
		int curIndex = -i * 75;
		DrawUtils::setColor(ColorUtil::rainbowColor(5, 1, 1, curIndex * 2).r, ColorUtil::rainbowColor(5, 1, 1, curIndex * 2).g, ColorUtil::rainbowColor(5, 1, 1, curIndex * 2).b, ColorUtil::rainbowColor(5, 1, 1, curIndex * 2).a);
		DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
		// DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.y - 1, pos.z - 1, pos.y), MC_Color(ColorUtil::astolfoRainbow(curIndex / 5, 1000).r, ColorUtil::astolfoRainbow(curIndex / 5, 1000).g, ColorUtil::astolfoRainbow(curIndex / 5, 1000).b, ColorUtil::astolfoRainbow(curIndex / 5, 1000).a));
		// DrawUtils::fillRectangleA(vec4_t(pos.x + 1, pos.w, pos.z - 1, pos.w + 1), MC_Color(ColorUtil::astolfoRainbow(curIndex / 5, 1000).r, ColorUtil::astolfoRainbow(curIndex / 5, 1000).g, ColorUtil::astolfoRainbow(curIndex / 5, 1000).b, ColorUtil::astolfoRainbow(curIndex / 5, 1000).a));
		pos.x += drawquality;
		pos.z += drawquality;
	}
}*/

void DrawUtils::drawRoundRectangle2(vec4_t pos, const MC_Color col) {
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 2, pos.z, pos.y - 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w + 1, pos.z, pos.w + 2), col);

	DrawUtils::fillRectangleA(vec4_t(pos.x - 1, pos.y - 1, pos.x, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.z, pos.y - 1, pos.z + 1, pos.w + 1), col);

	DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.w, pos.z, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.w, pos.x + 1, pos.w + 1), col);
	DrawUtils::fillRectangleA(vec4_t(pos.z - 1, pos.y - 1, pos.z, pos.y), col);
	DrawUtils::fillRectangleA(vec4_t(pos.x, pos.y - 1, pos.x + 1, pos.y), col);
}
inline void DrawUtils::tess__begin(Tessellator* tess, int vertexFormat, int numVerticesReserved) {
	__int64 tesselator = reinterpret_cast<__int64>(tess);

	using tess_begin_t = void(__fastcall*)(Tessellator*, int, int);
	static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(FindSignature("48 89 5C 24 ?? 55 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 45"));
	tess_begin(tess, vertexFormat, numVerticesReserved);
}
void DrawUtils::setGameRenderContext(__int64 ctx) {
	game3dContext = ctx;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->getOrigin();

	if(ctx){
		LARGE_INTEGER EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

		ElapsedMicroseconds.QuadPart *= 1000000;
		int ticksPerSecond = 20;
		if(g_Data.getClientInstance()->minecraft)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
		if(ticksPerSecond < 1)
			ticksPerSecond = 1;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
		lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
		if (lerpT > 1)
			lerpT = 1;
		else if (lerpT < 0)
			lerpT = 0;
	}
}
float DrawUtils::getLerpTime() {
	return lerpT;
}
vec3_t DrawUtils::getOrigin() {
	return origin;
}
void DrawUtils::drawLinestrip3d(const std::vector<vec3_t>& points) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = DrawUtils::get3dTessellator();

	DrawUtils::tess__begin(myTess, 5, (int)points.size());

	/*
	 * 1: quads
	 * 2: triangle list
	 * 3: trianglestrip (6)
	 * 4: line list
	 * 5: line strip (7)
	 */

	for(const auto& p : points){
		auto pD = p.sub(origin);
		tess_vertex(myTess, pD.x, pD.y, pD.z);
	}
	

	meshHelper_renderImm(game3dContext, myTess, entityFlatStaticMaterial);
}

MC_Color MC_Color::lerp(const MC_Color& o, float t) const {
	return MC_Color(Utils::lerp(r, o.r, t), Utils::lerp(g, o.g, t), Utils::lerp(b, o.b, t), Utils::lerp(a, o.a, t));
}
