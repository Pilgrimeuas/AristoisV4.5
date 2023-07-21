#pragma once
#include <sstream>

#include "ICommand.h"

class AutoTextCommand : public IMCCommand {
public:
	virtual bool execute(std::vector<std::string>* args) override;
	AutoTextCommand();
};