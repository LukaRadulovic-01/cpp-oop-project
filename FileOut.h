#pragma once
#include "OutStream.h"
#include <string>
#include <fstream>

class FileOut : public OutStream
{
	// ofstream file;
	string filename;
	bool doubleRedOut;
public:
	FileOut(const string& fileName, bool doubleRedOut);
	void write(const string& text) override;

};