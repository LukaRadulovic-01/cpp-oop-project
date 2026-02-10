#include "Tr.h"
#include <regex>

void TrCommand::execute(InStream& in, OutStream& out)
{
    string text = in.read();
    // string result;
    text = regex_replace(text, regex(what), with);

    out.write(text);
}
