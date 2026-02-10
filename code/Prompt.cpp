#include "Prompt.h"

extern string globalPrompt;

void PromptCommand::execute(InStream& in, OutStream& out)
{
	globalPrompt = arg;
}
