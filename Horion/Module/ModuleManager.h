#pragma once

#include <typeinfo>
#include <vector>
#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "../../Memory/GameData.h"

#ifdef _BETA
#include "Modules/Verification.h"
#endif

#include "Modules/AutoCity.h"
#include "Modules/Notifications.h"
#include "Modules/Interface.h"
#include "Modules/ArmorRepair.h"
#include "Modules/PacketMine.h"
#include "Modules/AntiCrystal.h"
//#include "Modules/APVPDisabler.h"
#include "Modules/RobotHolefiller.h"
#include "Modules/CityESP.h"
#include "Modules/MidClickPearl.h"
#include "Modules/Bhop.h"
//#include "Modules/RenSurround.h"
#include "Modules/SilentLook.h"
#include "Modules/TargetHud.h"
#include "Modules/AutoText.h"
//#include "Modules/FuckCity.h"
#include "Modules/InfoShow.h"
#include "Modules/Switcher.h"
#include "Modules/StrafeSpeed.h"
#include "Modules/Burrow.h"
#include "Modules/CevBreaker.h"
#include "Modules/PvPResources.h"
//#include "Modules/LavaPlacer.h"
#include "Modules/BlockEsp.h"
#include "Modules/AnvilAura.h"
#include "Modules/BowSpam.h"
#include "Modules/LiquidMove.h"
#include "Modules/ReverseStep.h"
#include "Modules/AutoWeb.h"

#include "Modules/CrystalAuraWTA.h"
#include "Modules/FastXPtwo.h"
//#include "Modules/BPGRewrite.h"
#include "Modules/CrystalUtilsJTWD.h"  // in progress
//#include "Modules/ManualCrystal.h"// fixed
#include "Modules/ExamModule.h"
#include "Modules/NoClip.h"		  // worksle
#include "Modules/AnchorAura.h"	  // broken
#include "Modules/AutoEZ.h"		  // retarded
#include "Modules/surround.h"     // fixed
#include "Modules/AutoTrap.h"	  // fixed
#include "Modules/HoleESP.h"      // fixed
#include "Modules/HoleFiller.h"	  // fixed
#include "Modules/NewBedAura.h"
#include "Modules//FastExp.h"
#include "Modules/Arraylist.h"

#include "Modules/Aimbot.h"
#include "Modules/AirSwim.h"
#include "Modules/AntiBot.h"
#include "Modules/AutoArmor.h"
//#include "Modules/AutoGapple.h"
#include "Modules/AutoSneak.h"
#include "Modules/AutoSprint.h"
#include "Modules/AutoTotem.h"
#include "Modules/BedAura.h"
#include "Modules/Fucker.h"
#include "Modules/Blink.h"
//#include "Modules/BowAimbot.h"
#include "Modules/StorageESP.h"
#include "Modules/ClickGuiMod.h"
#include "Modules/Teleport.h"
#include "Modules/Criticals.h"
#include "Modules/Placer.h"
//#include "Modules/CrystalAura.h"
//#include "Modules/CrystalAuraOW.h"
#include "Modules/Derp.h"
#include "Modules/ESP.h"
#include "Modules/FastEat.h"
#include "Modules/Fly.h"
//#include "Modules/AutoCrystal2.h"
//#include "Modules/AutoCrystal.h"
#include "Modules/HoleKicker.h"
#include "Modules/Freecam.h"
#include "Modules/FullBright.h"
#include "Modules/Glide.h"
//#include "Modules/FuckHead.h"
#include "Modules/PistonAura.h"
#include "Modules/Hitbox.h"
#include "Modules/SelfTrap.h"
#include "Modules/Killaura.h"
//#include "Modules/selectionHighlight.h"
#include "Modules/HudModule.h"
#include "Modules/ExtendedBlockReach.h"
//#include "Modules/InfiniteAura.h"
#include "Modules/InstaBreak.h"
#include "Modules/InventoryCleaner.h"
#include "Modules/InventoryMove.h"
#include "Modules/Jesus.h"
#include "Modules/Jetpack.h"
#include "Modules/Killaura.h"
#include "Modules/MidClick.h"
#include "Modules/Module.h"
#include "Modules/NameTags.h"
#include "Modules/NoFall.h"
#include "Modules/NoFriends.h"
#include "Modules/Velocity.h"
#include "Modules/NoPacket.h"
#include "Modules/NoSlowDown.h"
#include "Modules/NoWeb.h"
#include "Modules/Nuker.h"
#include "Modules/Phase.h"
#include "Modules/RainbowSky.h"
#include "Modules/Reach.h"
#include "Modules/Scaffold.h"
#include "Modules/Spammer.h"
#include "Modules/Speed.h"
#include "Modules/StackableItem.h"
#include "Modules/Step.h"
#include "Modules/TimeChanger.h"
#include "Modules/Timer.h"
#include "Modules/Tower.h"
#include "Modules/SessionInfo.h"
#include "Modules/ChatSuffix.h"
#include "Modules/Tracer.h"
//#include "Modules/TriggerBot.h"
#include "Modules/Xray.h"
#include "Modules/NightMode.h"
#include "Modules/NoSwing.h"
#include "Modules/Zoom.h"
#include "Modules/Teams.h"
#include "Modules/Nbt.h"
#include "Modules/NoHurtcam.h"
#include "Modules/FollowPathModule.h"
#include "Modules/Waypoints.h"
#include "Modules/Spider.h"
#include "Modules/Radar.h"
#include "Modules/ViewModel.h"
#include "Modules/FontChanger.h"
#include "Modules/Watermark.h"
#include "Modules/Theme.h"
#include "Modules/AutoCrystal.h"
#include "Modules/Coordinates.h"
#include "Modules/Keystroke.h"
#include "Modules/InvView.h"
#include "Modules/AutoRespawn.h"
#include "Modules/PlayerList.h"
#include "Modules/FastCrossbow.h"
#include "Modules/Welcome.h"
#include "Modules/AutoTool.h"
#include "Modules/AutoObsidian.h"
#include "Modules/Animations.h"
#include "Modules/Norender.h"
#include "Modules/RiptideFly.h"
#include "Modules/BigTest.h"
#ifdef _DEBUG
#include "Modules/PacketLogger.h"
#include "Modules/TestModule.h"
#endif


class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(C_GameMode* gameMode);
	void onAttack(C_Entity* attackedEnt);
	void onWorldTick(C_GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(C_MoveInputHandler* handler);
	void onPlayerTick(C_Player* plr);
	void onSendPacket(C_Packet*);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(moduleListMutex); }
	
	std::shared_mutex* getModuleListLock() { return &moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())){
				
				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
		
		auto lock = lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
