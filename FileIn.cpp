#include "FileIn.h"
#include <iostream>
#include <fstream>
#include <sstream>

FileIn::FileIn(const string& fileName) : filename(fileName)
{
    // file.open(fileName);
}

string FileIn::read()
{
    string text;
    ifstream file(filename);  // open each time
    if (!file.is_open()) {
        return "Can't open file\n";
    }
    ostringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();
    file.close();
	return text;
}
