#pragma once
#include "Command.h"
#include <string>

using namespace std;

class TrCommand : public Command
{
	string what;
	string with;
public:
	TrCommand(const string& argWhat, const string& argWith) : what(argWhat), with(argWith) {}
	void execute(InStream& in, OutStream& out) override;
};
