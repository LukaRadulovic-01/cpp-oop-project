#include "Wc.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>

void WcCommand::execute(InStream& in, OutStream& out)
{
	string text = in.read();

	if (option == "-w") {
		istringstream iss(text);
		int count = 0;
		string word;
		while (iss >> word) count++;
		out.write(to_string(count));
	}
	else if (option == "-c") {
		out.write(to_string((int)text.size()));
	}
}
