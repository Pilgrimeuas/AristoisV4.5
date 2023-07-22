#include <array>
#include <deque>
#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>  //radians
#include <unordered_set>

#include "../../../SDK/MatrixStack.h"
#include "../../../Utils/Json.hpp"
#include "../../../Utils/Logger.h"
#include "../../../Utils/VoxelIntersector.h"
#include "../../DrawUtils.h"
#include "../../path/JoeMovementController.h"
#include "../../path/JoePathFinder.h"
#include "../../path/goals/JoeGoal.h"
#include "../../path/goals/JoeGoalXZ.h"
#include "BigTest.h"

using json = nlohmann::json;

BigTest::BigTest() : IModule(0, Category::MISC, "For testing purposes") {
	registerEnumSetting("Enum1", &enum1, 0);
	enum1 = SettingEnum(this)
				.addEntry(EnumEntry("1", 1))
				.addEntry(EnumEntry("2", 2))
				.addEntry(EnumEntry("3", 3));
	registerEnumSetting("Enum2", &enum2, 0);
	enum2 = SettingEnum(this)
				.addEntry(EnumEntry("1", 1))
				.addEntry(EnumEntry("2", 2))
				.addEntry(EnumEntry("3", 3));
	registerFloatSetting("float1", &float1, 0, -10, 10);
	registerIntSetting("int1", &int1, 0, -10, 10);
	registerBoolSetting("bool1", &bool1, bool1);

	registerFloatSetting("float2", &float2, 0, -10, 10);
	registerIntSetting("int2", &int2, 0, -10, 10);
	registerBoolSetting("bool2", &bool2, bool2);

	registerFloatSetting("float3", &float3, 0, -10, 10);
	registerIntSetting("int3", &int3, 0, -10, 10);
	registerBoolSetting("bool3", &bool3, bool3);
}

BigTest::~BigTest() {
}

const char* BigTest::getModuleName() {
	return "BigTest";
}

bool BigTest::isFlashMode() {
	return false;
}

void BigTest::onEnable() {
}

void BigTest::onTick(C_GameMode* gm) {
	auto plr = g_Data.getLocalPlayer();
	if (plr == nullptr)
		return;
	if (plr->isInWater())
		clientMessageF("Water!");
}

void BigTest::onMove(C_MoveInputHandler* hand) {
}

void BigTest::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void BigTest::onSendPacket(C_Packet* p) {
}

void BigTest::onDisable() {
}

void BigTest::onLevelRender() {
}