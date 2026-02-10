#pragma once
#include "Command.h"
#include <vector>
#include <string>

using namespace std;

class EchoCommand : public Command {

public:
    
    void execute(InStream& in, OutStream& out) override;
};
