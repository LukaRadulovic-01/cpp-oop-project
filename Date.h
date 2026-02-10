#pragma once
#include "Command.h"

class DateCommand : public Command {
public:
    void execute(InStream& in, OutStream& out) override;
};
