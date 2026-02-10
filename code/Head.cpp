#include "Head.h"
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

void HeadCommand::execute(InStream& in, OutStream& out)
{
    string text = in.read();

    int n;
    string temp_option = option;
    temp_option.erase(0, 2);
    n = stoi(temp_option);

    istringstream iss(text);
    string line;
    int count = 0;
    ostringstream result;

    while (count < n && getline(iss, line)) {
        if (count == n - 1) {
            result << line;
        }
        else {
            result << line << "\n";
        }

        count++;
    }

    out.write(result.str());
}