#pragma once
#include "FileIn.h"
#include "FileOut.h"
#include <iostream>

FileOut::FileOut(const string& fileName, bool doubleRedOut) : filename(fileName), doubleRedOut(doubleRedOut)
{
	// file.open(fileName);
}

void FileOut::write(const string& text)
{
	if (!doubleRedOut) {
		ofstream file(filename);
		if (!file.is_open()) {
			cout << "Can't open file\n";
		}
		file << text;
		file.close();
	}
	else {
		ofstream file(filename, std::ios::app);
		if (!file.is_open()) {
			cout << "Can't open file\n";
		}
		file << text;
		file.close();
	}
}
