#include "Truncate.h"
#include <fstream>

void TruncateCommand::execute(InStream& in, OutStream& out)
{
    ifstream check(filename); // checking if file exists
    if (check.good()) {
        check.close();
        ofstream file(filename); // creating file
        if (!file) {
            out.write("Error: cannot create file\n");
        }
        check.close();
        return;
    }
    else {
        out.write("File does not exist\n");
    }
}
