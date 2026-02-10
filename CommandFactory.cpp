#include "CommandFactory.h"
#include "Echo.h"
#include "Prompt.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Truncate.h"
#include "Rm.h"
#include "Wc.h"
#include "Head.h"
#include "Batch.h"
#include "Tr.h"



unique_ptr<Command> CommandFactory::create(const CommandSpec& spec) {
    
    if (spec.name == "echo") return make_unique<EchoCommand>();
    if (spec.name == "prompt") return make_unique<PromptCommand>(spec.quotedArg);
    if (spec.name == "time") return make_unique<TimeCommand>();
    if (spec.name == "date") return make_unique<DateCommand>();
    if (spec.name == "touch") return make_unique<TouchCommand>(spec.fileName);
    if (spec.name == "truncate") return make_unique<TruncateCommand>(spec.fileName);
    if (spec.name == "rm") return make_unique<RmCommand>(spec.fileName);
    if (spec.name == "wc") return make_unique<WcCommand>(spec.option);
    if (spec.name == "head") return make_unique<HeadCommand>(spec.option);
    if (spec.name == "batch") return make_unique<BatchCommand>();
    if (spec.name == "tr" && spec.args.empty()) return make_unique<TrCommand>(spec.quotedArg, "");
    if (spec.name == "tr" && spec.args.size() == 1) return make_unique<TrCommand>(spec.quotedArg, spec.args[0]);
    if (spec.name == "tr" && spec.args.size() == 2) return make_unique<TrCommand>(spec.args[0], spec.args[1]);

    return nullptr;
}

