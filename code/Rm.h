#pragma once
#include "Command.h"
#include <string>

using namespace std;

class RmCommand : public Command {
    string filename;
public:
    RmCommand(const string& f) : filename(f) {}
    void execute(InStream& in, OutStream& out) override;

};
