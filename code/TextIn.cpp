#include "TextIn.h"

TextIn::TextIn(const string& text)
{
	data = text;
}

string TextIn::read()
{
	return data;
}
