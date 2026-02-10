#pragma once
#include <vector>
#include <string>

using namespace std;

class CommandIOStream
{
public:
	vector<string> choseStream(const string& name, const string& line, bool redIn, bool redOut);
	vector<string> chosePipeStream(const string& name, const string& line, bool redIn, bool redOut);
};