#pragma once
#include "Command.h"

class TimeCommand : public Command {
public:
    void execute(InStream& in, OutStream& out) override;
};


