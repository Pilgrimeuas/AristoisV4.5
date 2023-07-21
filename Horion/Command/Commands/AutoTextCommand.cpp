#include "../../Module/ModuleManager.h"
#include "AutoTextCommand.h"

AutoTextCommand::AutoTextCommand() : IMCCommand("AutoText", "AutoText", "<set/reset> <text>") {
	registerAlias("at");
	registerAlias("text");
}

bool AutoTextCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);
	//auto interfaceMod = moduleMgr->getModule<Interface>();
	auto SBMOD = moduleMgr->getModule<AutoText>();

	if (args->at(1) == "set" && args->size() > 2) {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 1)
				os << " ";
			os << args->at(i);
		}
		std::string text = os.str().substr(1);
		std::string setStr = "change to " + args->at(2) + "!";
		clientMessageF("%s%s", GREEN, "Successfully Set!");
		// interfaceMod->getClientName() = text;
		SBMOD->getClientName() = text;
		return true;
	} else if (args->at(1) == "reset") {
		// interfaceMod->getClientName() = "Packet Client";
		SBMOD->getClientName() = "/hub";
		clientMessageF("%s%s", GREEN, "Successfully Rest!");
		return true;
	}
	return false;
}