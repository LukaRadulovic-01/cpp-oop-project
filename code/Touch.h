#pragma once
#include "Command.h"
#include <string>

using namespace std;

class TouchCommand : public Command {
    string filename;
public:
    TouchCommand(const string& f) : filename(f) {}
    void execute(InStream& in, OutStream& out) override;
};
