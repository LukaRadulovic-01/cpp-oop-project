#pragma once
#include "Command.h"
#include <string>
#include <vector>

using namespace std;

class BatchCommand : public Command {
public:
    void execute(InStream& in, OutStream& output) override;
};
