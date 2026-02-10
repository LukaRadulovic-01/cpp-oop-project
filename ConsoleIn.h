#pragma once
#include "InStream.h"
#include <string>

using namespace std;

class ConsoleIn : public InStream
{
public:
	string read() override;

};
