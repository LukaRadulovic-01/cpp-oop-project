#include "PipeIn.h"

PipeIn::PipeIn(const string& input) : data(input) {}

string PipeIn::read()
{
	return data;
}
