#pragma once
#include "OutStream.h"
#include <string>


class ConsoleOut : public OutStream
{
public:
	void write(const string& text) override;
};