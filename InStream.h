#pragma once
#include <string>

using namespace std;

class InStream
{
public:
	virtual string read() = 0;
	virtual ~InStream() = default;
};