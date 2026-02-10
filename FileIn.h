#pragma once
#include "InStream.h"
#include <fstream>

using namespace std;

class FileIn : public InStream {
	// ifstream file;
	string filename;
public:
	FileIn(const string& fileName);
	string read() override;
};
