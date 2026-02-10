#pragma once
#include "InStream.h"
#include "OutStream.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(InStream& in, OutStream& out) = 0;
};