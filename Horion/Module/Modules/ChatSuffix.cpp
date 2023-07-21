#include "ChatSuffix.h"

ChatSuffix::ChatSuffix() : IModule(0, Category::MISC, "Adds the client suffix") {
	registerBoolSetting("Green", &green, green);
}

ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "ChatSuffix";
}

void ChatSuffix::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
		std::string Sentence;
		std::string end;
		std::string green1 = ">";
		 end = " | AristoisV3";
#ifdef _DEBUG
		end += " | AristoisV3 Beta";
#endif  // _DEBUG
		if(green)
		Sentence = green1 + funy->message.getText() + end;
		else
		Sentence = funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}
