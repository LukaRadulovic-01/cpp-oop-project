#pragma once
#include <memory>
#include "Command.h"
#include "Parser.h"
#include <string>

using namespace std;

class CommandFactory {
public:
    unique_ptr<Command> create(const CommandSpec& spec);
};