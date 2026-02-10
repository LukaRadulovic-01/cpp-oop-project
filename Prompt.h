#pragma once
#include "Command.h"
#include <string>

using namespace std;

class PromptCommand : public Command {
	string arg;
public:
	PromptCommand(const string& p) : arg(p) {}
	void execute(InStream& in, OutStream& out) override;

};



