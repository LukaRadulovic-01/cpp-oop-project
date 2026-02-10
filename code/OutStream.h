#pragma once
#include <string>

using namespace std;

class OutStream
{
public:
	virtual void write(const string& test) = 0;
	virtual string getContent() const { return ""; }
	virtual ~OutStream() = default;

};