#pragma once
#include "Command.h"
#include <vector>
#include <string>

using namespace std;

class WcCommand : public Command {
    string option;
public:
    WcCommand(const string& opt) : option(opt) {}
    void execute(InStream& in, OutStream& out) override;
};
