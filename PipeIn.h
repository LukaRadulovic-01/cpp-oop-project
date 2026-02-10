#pragma once
#include "InStream.h"
#include <string>

class PipeIn : public InStream
{
	string data;
public:
	PipeIn(const string& input);
	string read() override;
};
