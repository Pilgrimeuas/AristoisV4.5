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
#include "../ModuleManager.h"

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
	registerIntSetting("int1", &int1, 0, 0, 101);
	registerBoolSetting("bool1", &bool1, bool1);

	registerFloatSetting("float2", &float2, 0, -10, 10);
	registerIntSetting("int2", &int2, 0, 0, 101);
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
static std::vector<C_Entity*> testlist;
void TestFind(C_Entity* currentEntity, bool isRegularEntity) {
	static auto testmode = moduleMgr->getModule<BigTest>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (!Target::isValidTarget(currentEntity))
		return;
	float dist = (currentEntity->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());  // 更加曼妙的玩家定位
	if (dist < 10) {
		testlist.push_back(currentEntity);  // 如果目标远处距离就返回
	}
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
	C_ItemStack* current;
	std::string print1 = "@";
	std::string name;
	std::string print2 = " TotemPop: ";
	std::string count = std::to_string(int1);
	testlist.clear();
	g_Data.forEachEntity(TestFind);
	int3++;
	if (!testlist.empty() && int3 > float1) {
		name = testlist[0]->getNameTag()->getText();

		name = Utils::sanitize(name);

		vec3_t targetpos = *testlist[0]->getPos();
		int health = testlist[0]->getHealth();

		current = testlist[0]->getEquippedTotem();
		
		if (current->item == NULL && int1 == int2) {
			bool1 = true;
		}
		int3 = 0;
	}

	//send packet
	if (bool1) {
		int1 = int2 + 1;
			std::string text = print1 + name + print2 + count;
			C_TextPacket textPacket;
			textPacket.message.setText(text);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			bool1 = false;
	} else
			int2 = int1;
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