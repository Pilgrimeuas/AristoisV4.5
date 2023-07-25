#pragma once
#include "../ModuleManager.h"
#include "Module.h"

struct CrystalPlacements {
	vec3_t toPlace;
	float enemyDmg = 0.f;
	float selfDmg = 0.f;
};

struct CrystalInfo {
	CrystalPlacements CPlacements;
	C_Entity* ent;
};

struct lineResults {
	float blockCount = 0;
	vec3_t lastSolidBlock;
};

inline bool checkCornerHitboxCollision(vec3_t* block, C_Entity* ent) {  // THANK SB HOLY SHIT I WAS TRYING TO MAKE THIS BY MYSELF FOR HOURS!!11!1
	std::vector<vec3_t*> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));

	if (ent->getEntityTypeId() != 319) {
		if (!corners.empty()) {
			for (auto corner : corners) {
				if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
					return true;
				}
			}
		}
	} else {
		vec3_t pp = ent->getHumanPos();
		vec3_t entCorners[9] = {
			/*			pp.add(.3f, 0, .3f),
			pp.add(-.3f, 0, .3f),
			pp.add(.3f, 0, -.3f),
			pp.add(-.3f, 0, -.3f),*/
			pp.add(0, -1.f, 0),
			pp.add(.284f, 0, .284f),
			pp.add(-.284f, 0, .284f),
			pp.add(.284f, 0, -.284f),
			pp.add(-.284f, 0, -.284f),
		};

		for (vec3_t i : entCorners) {
			if (i.floor() == *block) {
				return true;
			}
		}
	}

	return false;
}

inline lineResults countBlksAlongLine(vec3_t start, vec3_t end) {
	vec3_t endf = end.floor();
	vec3_t startf = start.floor();

	lineResults rtn;

	if (endf == startf) {
		rtn.blockCount = 0.f;
		rtn.lastSolidBlock = startf;
		return rtn;
	}

	float dist = startf.dist(endf);

	float dX = endf.x - startf.x;
	float dY = endf.y - startf.y;
	float dZ = endf.z - startf.z;

	int steps = 200;
	const float segment = dist / steps;
	const vec3_t seggs3d = vec3_t((dX / steps), (dY / steps), (dZ / steps));

	vec3_t imHere = startf;
	vec3_ti inCoord = startf;

	while (steps-- >= 0) {
		// if ((vec3_ti)imHere.floor() != inCoord) {
		inCoord = imHere.floor();

		if (!g_Data.getLocalPlayer()->region->getBlock(inCoord)->blockLegacy->material->isReplaceable) {  // if inCoord is a block
			rtn.blockCount = rtn.blockCount + segment;
			rtn.lastSolidBlock = inCoord.toVec3t();
		}
		//}

		imHere = imHere.add(seggs3d);
	}
	return rtn;
}

class CrystalPlace : public IModule {
private:
	int origSlot;
	int ctr = 0;

	vec3_t toPlace;
	bool currentlySwitchingHotbar = false;
	float dmg_nukkit = false;

	int oldSlot;

	int Mdel = 0;  // main delay
	int Rdel = 0;  // retry delay

public:
	vec2_t rotAngleB;
	bool breakArrEmpty = false;

	int aSlot;
	int breakdelay = 0;  // Time to wait (in ticks) until to break a new crystal

	bool retardBreak = false;  // 弱智般破坏水晶但是提高速度

	float breakRange = 6.f;  // 破坏范围

	bool breakAll = true;  // 破坏全部水晶

	float breakWalls = 10;  // 能穿过多少墙破坏水晶
	float postBWalls = 10;  // 最多能穿过多少堵墙的距离

	float breakHealth = 5.f;  // 多少伤害停止破坏水晶
	float BmaxSelfDmg = 5.f;  // 破坏水晶最多对自己造成多少伤害来决定破不破坏这颗水晶

	bool antiWeakness = false;  // 防止傻逼虚弱狗还没有人修啊

	bool renderBreaking = true;  // 渲染破坏水晶方框
	float Opacity = 1.f; // 渲染透明度
	SettingEnum rotateBreak;  // 破坏转向 (none->obvious
							  //			  正常转向
							  //			  发包转向

	bool configHelper = true; //不清楚有什么用

	std::vector<CrystalInfo> CJTWDPlaceArr;
	vec2_t rotAngle;
	bool placeArrEmpty = false;
	bool rotUpNow = false;

	int delay = 0;  // 延迟多久放水晶
	// int placetimes = 1;     // Number of times the client should keep retrying to place a crystal
	int maxProximity = 4;    // 在切换轴之前，水晶与人之间的最大距离是多少
	float range = 8;         // 要添加到目标列表的目标的范围人话就是目标范围
	float placeRange = 6.f;  // 水晶最大的放置范围
	SettingEnum priority;    // 决定如何对目标进行排序(距离->从最低到最高
							 //)
	float thruWallsR = 6;    // 允许您放置多少水晶穿过墙壁
	float postWallsR = 10;   // 穿墙后放置的最大距离

	/* SOON
	int lookAhead = 5;		// how many ticks in the future to predict future enemy location (set to 0 to disable predict)
	*/

	SettingEnum calcDmgType;  // 用什么方法来计算损害（java，基岩版）

	bool safetyFirst = false;  // 是否优先考虑(减少)自我伤害而不是(最大化)敌人的伤害
	float minHealth = 5.f;     // 你应该停止放置水晶的最低生命水平是多少
	float maxSelfDmg = 5.f;    // 允许自己受到的最大伤害
	float minEnemDmg = 10.f;   // 水晶对敌人造成的伤害，最低限度
	SettingEnum facePlaceType;  // 控制炸脸放置(none->无脑
//SMART->仅当敌人处于特定生命状态时才炸脸
	float fpThresh = 20.f;      // 仅当facePlaceType设置为SMART(如果设置为20)时，client开始炸脸
	float dmgAtThresh = 3.f;    // 当达到fpThreshold时，水晶需要提供多大的损害

	bool renderPlacing = true;  // 是否渲染放置位置
	bool noCheckUpper = false;  // 如果水晶可以放在方块下面（endzone 1.13+ java）人话就是碰撞箱大小确定
	bool attackMulti = false;   // 自己是否应该尝试攻击1个以上的敌人
	SettingEnum switchType;     // 切换模式(reg->在启动时切换到水晶
				 			//                 switchback->类似于物品栏在循环切换
								//                 APVPSpoof->鬼手

	SettingEnum rotate;  // Rotations (none->obvious
						 //			  normal->正常转向
						 //			  silent发包转向;
						 //           pitchUp->抬头转向，但是我们已经bypass了所以这个选项忽略lol
	void sortCrystals();
	// void findAFuckingWeapon();
	/* TODO
	 *  when APVPSpoof on, stop crystalling on mine
	 *  fix crashing when there is no crystal and apvpspoof is on
	 *  steal silent rots from ng
	 */

	/* TODO (in CrystalBreak):
	 * anti-weakness
	 * anti blocking crystals
	 */

	CrystalPlace();
	~CrystalPlace();

	bool isPlaceValid(vec3_t location, C_Entity* atkObj);
	bool tryRaytrace(vec3_t pos, vec3_t end, C_BlockSource* region);
	// T H E  H O L Y  T R I N I T Y
	float computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg);
	float getBlastDamageEnchantReduction(C_ItemStack* armor);
	// float calculateBlockDensity(C_Entity* targetEntity, vec3_t explosionPosition);

	std::vector<CrystalPlacements> generateValidPlacements(C_Entity* target, int yOffset);

	CrystalPlacements bestPlaceOnPlane(C_Entity* target, int yOffset);

	CrystalPlacements CrystalAuraJTWD(C_Entity* target);

	bool getCrystalSpoof();

	//  via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	// virtual void onPlayerTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet) override;
};

/*class CrystalBreak : public IModule {
private:
	int oldSlot;

	int Mdel = 0;  // main delay
	int Rdel = 0;  // retry delay

public:
	vec2_t rotAngleB;
	bool breakArrEmpty = false;

	int aSlot;

	int breakdelay = 0;  // Time to wait (in ticks) until to break a new crystal

	bool retardBreak = false; // weird super fast break shit

	float breakRange = 6.f;  // Range to break endCrystals

	bool breakAll = true;  // Whether to break ALL the crystals or just the nearest one

	float breakWalls = 10;    // How many blocks you are allowed to place through walls
	float postBWalls = 10;  // Maximum distance to place *after* going through a wall

	float breakHealth = 5.f;  // What is the minimum health you should have to stop breaking crystals
	float BmaxSelfDmg = 5.f;  // Maximum damage allowable to player

	bool antiWeakness = false;  // detects if you have been hit by weakness and switches to a weapon accordingly

	bool renderBreaking = true;  // Whether to render crystal break attempts

	SettingEnum rotateBreak;  // Rotations (none->obvious
							  //			  normal->clientside rotate
							  //			  silent->sends packet to go look at the crystal;

	bool configHelper = true;

	void sortCrystals();
	void findAFuckingWeapon();

	CrystalBreak();
	~CrystalBreak();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onLevelRender();
	virtual void onSendPacket(C_Packet* packet) override;

};*/