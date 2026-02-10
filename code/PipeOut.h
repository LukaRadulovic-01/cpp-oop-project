#pragma once
#include "OutStream.h"
#include <string>

class PipeOut : public OutStream
{
	string buffer;
public:
	void write(const string& text) override;
	string getContent() const override;
};