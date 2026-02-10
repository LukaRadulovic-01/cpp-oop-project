#include "Rm.h"
#include <fstream>
#include <filesystem>
#include <cstdio> // For std::remove

using namespace std;

void RmCommand::execute(InStream& in, OutStream& out)
{
    ifstream check(filename);
    if (check.good()) {
        check.close();
        if (remove(filename.c_str()) == 0) {
            // std::remove returns 0 on success
            // out.write("File deleted successfully\n");
        }
        else {
            out.write("Error deleting file.\n");
        }
        return;
    }
    else {
        out.write("File does not exist\n");
    }
}
