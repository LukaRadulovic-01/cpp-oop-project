#include "PipeOut.h"

void PipeOut::write(const string& text)
{
	buffer = text;
}

string PipeOut::getContent() const
{
	return buffer;
}
