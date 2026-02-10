#include "ConsoleIn.h"
#include <iostream>


string ConsoleIn::read()
{
    string line;
    string result = "";
    bool endOfFile = false;
    while (true) {
        getline(cin, line);
        if (cin.eof()) {
            endOfFile = !endOfFile;
            cin.clear();
            result.pop_back();
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        result += line + "\n";
        
    }
    return result;
}
