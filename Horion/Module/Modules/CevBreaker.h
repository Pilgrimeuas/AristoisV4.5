#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class CevBreaker : public IModule {
	private:
		bool Clicked = false;
		bool silent = false;
		bool airplace = false;
		bool swaped = false;

		bool hasdone = false;

		bool hasplaceob = false;
		bool hasplacecry = false;
		bool hasbreak = false;
		bool haspick = false;
		//bool hastrap = false;

		//
		bool Dhasob = false;
		bool DhascRY = false;
		bool Dhaspick = false;
		//bool Dhasbb = false;
		//bool Dtrap = false;
		//
		int tickTimer = 0;
		bool takenob = false;
		bool takencry = false;
		bool takenpick = false;
		//bool takentrap = false;
		//bool anti = false;
	public:
		int cdelay = 0;
		int pdelay = 1;
		int odelay = 0;
		int bdelay =2;
		int idk = 0;
		vec3_ti pos;
		vec3_ti pos1;
		vec3_ti pos2;
		vec3_ti pos3;
		vec3_ti pos4;
		int odelays = 0;
		int delay = 0;
		float range = 10.f;
		CevBreaker();
		~CevBreaker();
		virtual const char* getModuleName() override;
		virtual void findWeapon();
		virtual void onEnable() override;
		virtual void onSendPacket(C_Packet* packet) override;
		virtual void onTick(C_GameMode* gm) override;
		virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	};