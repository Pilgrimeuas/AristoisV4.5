#pragma once
#include "Module.h"
class ChatSuffix : public IModule {
public:
	bool green = true;
	ChatSuffix();
	~ChatSuffix();

	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};