#include "CommandIOStream.h"
#include <regex>

using namespace std;

vector<string> CommandIOStream::choseStream(const string& name, const string& line, bool redIn, bool redOut)
{
    string in;
    string out;
    vector<string> inOut;
    string flagIn = "flagIn";
    string flagOut = "flagOut";

    if (name == "echo") {
        // echo file.txt
        regex patternFile(R"(^echo\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn) {
            in = "file";
            out = redOut ? "file" : "console";
        }

        // echo "text"
        regex patternText(R"(^echo\s+"[^"]*"\s*$)");
        if (regex_match(line, patternText) && !redIn) {
            in = "text";
            out = redOut ? "file" : "console";
        }

        // echo
        regex patternEmpty(R"(^echo\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = redIn ? "file" : "console";
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "prompt") {
        // prompt "text"
        regex patternText(R"(^prompt\s+"[^"]*"\s*$)");
        if (regex_match(line, patternText) && !redIn) {
            in = "text";
            // command has no output
        }
    }
    else if (name == "time") {
        // time
        regex patternEmpty(R"(^time\s*$)");
        if (regex_match(line, patternEmpty) && !redIn) {
            // command has no input
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "date") {
        // date
        regex patternEmpty(R"(^date\s*$)");
        if (regex_match(line, patternEmpty) && !redIn) {
            // command has no input
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "touch") {
        // touch file.txt
        regex patternFile(R"(^touch\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn && !redOut) {
            in = "file";
            // command has no output
        }
    }
    else if (name == "truncate") {
        // truncate file.txt
        regex patternFile(R"(^truncate\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn && !redOut) {
            in = "file";
            // command has no output
        }
    }
    else if (name == "rm") {
        // rm file.txt
        regex patternFile(R"(^rm\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn && !redOut) {
            in = "file";
            // command has no output
        }
    }
    else if (name == "wc") {
        // wc -w/c file.txt
        regex patternFile(R"(^wc\s+-(w|c)\s+[a-zA-Z0-9_\-\.]+\.(txt)\s*$)");
        if (regex_match(line, patternFile) && !redIn) {
            in = "file";
            out = redOut ? "file" : "console";
        }
        // wc -w/c "text"
        regex patternText(R"(^wc\s+-(w|c)\s+"[^"]*"\s*$)");
        if (regex_match(line, patternText) && !redIn) {
            in = "text";
            out = redOut ? "file" : "console";
        }
        // wc -w/c
        regex patternEmpty(R"(^wc\s+-(w|c)\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = redIn ? "file" : "console";
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "head") {
        // head -ncount file.txt
        //([^\s]+\.txt)$) moze i ovo
        regex patternFile(R"(^head\s+-n(0|[1-9][0-9]{0,4})\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn) {
            in = "file";
            out = redOut ? "file" : "console";
        }
        // head -ncount "text"
        regex patternText(R"(^head\s+-n(0|[1-9][0-9]{0,4})\s+"[^"]*"\s*$)");
        if (regex_match(line, patternText) && !redIn) {
            in = "text";
            out = redOut ? "file" : "console";
        }
        // head -ncount
        regex patternEmpty(R"(^head\s+-n(0|[1-9][0-9]{0,4})\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = redIn ? "file" : "console";
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "batch") {
        // batch file.txt
        regex patternFile(R"(^batch\s+[a-zA-Z0-9_\-\.]+\.txt\s*$)");
        if (regex_match(line, patternFile) && !redIn) {
            in = "file";
            out = redOut ? "file" : "console";
        }
        // batch
        regex patternEmpty(R"(^batch\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = redIn ? "file" : "console";
            out = redOut ? "file" : "console";
        }
    }
    else if (name == "tr") {
        // tr file.txt what with 
        // moze i ovo [^\s]
        regex patternFile(R"(^tr\s+[a-zA-Z0-9_\-\.]+\.txt\s+"[^"]*"(?:\s+"[^"]*")?\s*$)");
        if (regex_match(line, patternFile) && !redIn) {
            in = "file";
            out = redOut ? "file" : "console";
        }
        // tr "text" what with
        regex patternText(R"(^tr\s+"[^"]*"\s+"[^"]*"(?:\s+"[^"]*")?\s*$)");
        if (regex_match(line, patternText) && !redIn) {
            in = "text";
            out = redOut ? "file" : "console";
        }
        // tr what with
        regex patternEmpty(R"(^tr\s+"[^"]*"(?:\s+"[^"]*")?\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = redIn ? "file" : "console";
            out = redOut ? "file" : "console";
        }
    }
    else {
        return inOut;
    }

    if (!in.empty()) { 
        inOut.push_back(in);
        inOut.push_back(flagIn);
    }
    if (!out.empty()) {
        inOut.push_back(out);
        inOut.push_back(flagOut);
    }

    return inOut;
}

vector<string> CommandIOStream::chosePipeStream(const string& name, const string& line, bool redIn, bool redOut)
{
    string in;
    string out;
    vector<string> inOut;

    if (name == "echo" && !redIn && !redOut) {
        // echo
        regex patternEmpty(R"(^echo\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In";
            out = "Pipe Out";
        }
    }
    else if (name == "prompt" && !redIn && !redOut) {
        // prompt
        regex patternText(R"(^prompt\s*$)");
        if (regex_match(line, patternText)) {
            in = "Pipe In";
            out = "Pipe Out"; // command has no output
        }
    }
    else if (name == "time" && !redIn && !redOut) {
        // time
        regex patternEmpty(R"(^time\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In"; // command has no input
            out = "Pipe Out";
        }
    }
    else if (name == "date" && !redIn && !redOut) {
        // date
        regex patternEmpty(R"(^date\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In"; // command has no input
            out = "Pipe Out";
        }
    }
    else if (name == "touch" && !redIn && !redOut) {
        // touch
        regex patternFile(R"(^touch\s*$)");
        if (regex_match(line, patternFile)) {
            in = "Pipe In";
            out = "Pipe Out"; // command has no output
        }
    }
    else if (name == "truncate" && !redIn && !redOut) {
        // truncate
        regex patternFile(R"(^truncate\s*$)");
        if (regex_match(line, patternFile)) {
            in = "Pipe In";
            out = "Pipe Out"; // command has no output
        }
    }
    else if (name == "rm" && !redIn && !redOut) {
        // rm
        regex patternFile(R"(^rm\s*$)");
        if (regex_match(line, patternFile)) {
            in = "Pipe In";
            out = "Pipe Out"; // command has no output
        }
    }
    else if (name == "wc" && !redIn && !redOut) {
        // wc -w/c
        regex patternEmpty(R"(^wc\s+-(w|c)\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In";
            out = "Pipe Out";
        }
    }
    else if (name == "head" && !redIn && !redOut) {
        // head -ncount
        regex patternEmpty(R"(^head\s+-n(0|[1-9][0-9]{0,4})\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In";
            out = "Pipe Out";
        }
    }
    else if (name == "batch" && !redIn && !redOut) {
        // batch
        regex patternEmpty(R"(^batch\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In";
            out = "Pipe Out";
        }
    }
    else if (name == "tr" && !redIn && !redOut) {
        // tr what with
        regex patternEmpty(R"(^tr\s+"[^"]*"(?:\s+"[^"]*")?\s*$)");
        if (regex_match(line, patternEmpty)) {
            in = "Pipe In";
            out = "Pipe Out";
        }
    }
    else {
        return inOut;
    }

    return inOut;


    return vector<string>();
}
