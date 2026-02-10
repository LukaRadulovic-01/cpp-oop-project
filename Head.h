#pragma once
#include "Command.h"
#include <string>
#include <vector>

using namespace std;

class HeadCommand : public Command {
    string option;
public:
    HeadCommand(const string& opt) : option(opt) {}

    void execute(InStream& in, OutStream& out) override;
};