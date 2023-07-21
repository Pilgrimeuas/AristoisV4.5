#include "AutoText.h"

AutoText::AutoText() : IModule(0, Category::PLAYER, "Use .Autotext set <words>") {
	mode = SettingEnum(this)
			   .addEntry(EnumEntry("Text", 0))
			   .addEntry(EnumEntry("Command", 1));
				registerEnumSetting("Mode", &this->mode, 0);
}
AutoText::~AutoText() {
}
const char* AutoText::getModuleName() {
	return "AutoText";
}
void AutoText::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;


		if (!hasClicked) {
		if (mode.getSelectedValue() == 0) {
			C_TextPacket textPacket;
			textPacket.message.setText(textstring);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		}
		if (mode.getSelectedValue() == 1) {
			CommandRequestPacket packet;
			TextHolder cmd = textstring;
			packet.command = cmd;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
		}
			hasClicked = true;

		} else if (hasClicked) {

			this->setEnabled(false);
			hasClicked = false;
		}
}