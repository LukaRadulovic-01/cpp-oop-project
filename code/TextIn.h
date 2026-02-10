#pragma once
#include "InStream.h"


class TextIn : public InStream
{
	string data;
public:
	TextIn(const string& text);
	string read() override;
};