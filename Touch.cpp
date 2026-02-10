#include "Touch.h"
#include <fstream>

using namespace std;

void TouchCommand::execute(InStream& in, OutStream& out) {
    ifstream check(filename); // checking if file exists
    if (check.good()) {
        out.write("Error: file already exists");
        return;
    }
    check.close();
    ofstream file(filename); // creating file
    if (!file) {
        out.write("Error: cannot create file");
    }
    check.close();
}
