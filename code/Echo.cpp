#include "Echo.h"
#include <string>

void EchoCommand::execute(InStream& in, OutStream& out)
{
	string text = in.read();
	out.write(text);
}
