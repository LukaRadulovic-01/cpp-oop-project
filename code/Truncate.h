#pragma once
#include "Command.h"
#include <string>

using namespace std;

class TruncateCommand : public Command {
    string filename;
public:
    TruncateCommand(const string& f) : filename(f) {}
    void execute(InStream& in, OutStream& out) override;
};
