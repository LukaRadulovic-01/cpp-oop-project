#pragma once
#include <string>
#include <vector>

using namespace std;

struct CommandSpec {
    string name;
    string quotedArg;
    string option;
    string fileName;
    vector<string> args;
    string redIn;
    string redOut;
    bool valid;
};

class Parser {
public:
    CommandSpec parse(const string& line);

};
