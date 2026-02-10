#include "Parser.h"
#include <cctype>

using namespace std;

enum class Mode {
    NORMAL,
    REDIN_STREAM,
    REDOUT_STREAM
};

static void processToken(const string& token, CommandSpec& spec, Mode mode) {
    if (token.empty()) return;

    // Streaming modes: every new token updates redIn or redOut
    if (mode == Mode::REDIN_STREAM) {
        spec.redIn = token;
        return;
    }
    if (mode == Mode::REDOUT_STREAM) {
        spec.redOut = token;
        return;
    }

    // Normal mode rules:
    // 1) redIn like <file.txt
    if (token[0] == '<' &&
        token.size() >= 5 &&
        token.substr(token.size() - 4) == ".txt" &&
        spec.redIn.empty()) {
        spec.redIn = token.substr(1);
        return;
    }

    // 2) redOut like >file.txt or >>file.txt
    if ((token.rfind(">>", 0) == 0 || token.rfind(">", 0) == 0) &&
        spec.redOut.empty()) {
        if (token.rfind(">>", 0) == 0)
            spec.redOut = token.substr(2);
        else
            spec.redOut = token.substr(1);
        return;
    }

    // 3) name
    if (spec.name.empty()) {
        spec.name = token;
        return;
    }

    // 4) option
    if (token[0] == '-' && spec.option.empty()) {
        spec.option = token;
        return;
    }

    // 5) fileName (first .txt not redIn/redOut)
    if (token.size() >= 4 &&
        token.substr(token.size() - 4) == ".txt" &&
        spec.fileName.empty()) {
        spec.fileName = token;
        return;
    }

    // 6) everything else - args
    spec.args.push_back(token);
}

CommandSpec Parser::parse(const string& line) {
    CommandSpec spec;
    spec.valid = true;

    string token;
    bool inQuotes = false;
    string currentQuoted;
    Mode mode = Mode::NORMAL;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            if (!inQuotes) {
                if (spec.quotedArg.empty())
                    spec.quotedArg = currentQuoted;
                else
                    spec.args.push_back(currentQuoted);
                currentQuoted.clear();
            }
            continue;
        }

        if (inQuotes) {
            currentQuoted.push_back(c);
            continue;
        }

        if (isspace(c)) {
            if (!token.empty()) {
                // Check for standalone operators that switch mode
                if (token == "<") {
                    mode = Mode::REDIN_STREAM;
                }
                else if (token == ">" || token == ">>") {
                    mode = Mode::REDOUT_STREAM;
                }
                else {
                    processToken(token, spec, mode);
                }
                token.clear();
            }
        }
        else {
            token.push_back(c);
        }
    }

    if (!token.empty()) {
        if (token == "<") {
            mode = Mode::REDIN_STREAM;
        }
        else if (token == ">" || token == ">>") {
            mode = Mode::REDOUT_STREAM;
        }
        else {
            processToken(token, spec, mode);
        }
    }

    if (inQuotes) // unclosed quotes
        spec.valid = false;

    return spec;
}









// originalni parser
/*#include "Parser.h"
#include <cctype>

using namespace std;

static void processToken(const string& token, CommandSpec& spec) {
    if (token.empty()) return;

    // 1) redIn (mora da pocinje sa < i zavrsava se sa .txt)
    if (token[0] == '<' &&
        token.size() >= 5 &&
        token.substr(token.size() - 4) == ".txt" &&
        spec.redIn.empty()) {
        spec.redIn = token.substr(1); // bez '<'
        return;
    }

    // 2) redOut (mora da pocinje sa > ili >>)
    if ((token.rfind(">>", 0) == 0 || token.rfind(">", 0) == 0) &&
        spec.redOut.empty()) {
        if (token.rfind(">>", 0) == 0)
            spec.redOut = token.substr(2);
        else
            spec.redOut = token.substr(1);
        return;
    }

    // 3) name (prva rec)
    if (spec.name.empty()) {
        spec.name = token;
        return;
    }

    // 4) option (prvi token sa -)
    if (token[0] == '-' && spec.option.empty()) {
        spec.option = token;
        return;
    }

    // 5) fileName (prvi .txt token koji nije redIn)
    if (token.size() >= 4 &&
        token.substr(token.size() - 4) == ".txt" &&
        spec.fileName.empty()) {
        spec.fileName = token;
        return;
    }

    // 6) sve ostalo ide u args
    spec.args.push_back(token);
}

CommandSpec Parser::parse(const string& line) {
    CommandSpec spec;
    spec.valid = true;

    string token;
    bool inQuotes = false;
    string currentQuoted;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            if (!inQuotes) {
                if (spec.quotedArg.empty())
                    spec.quotedArg = currentQuoted;
                else
                    spec.args.push_back(currentQuoted);
                currentQuoted.clear();
            }
            continue;
        }

        if (inQuotes) {
            currentQuoted.push_back(c);
            continue;
        }

        if (isspace(c)) {
            if (!token.empty()) {
                processToken(token, spec);
                token.clear();
            }
        }
        else {
            token.push_back(c);
        }
    }

    if (!token.empty())
        processToken(token, spec);

    if (inQuotes) // ne zatvoreni navodnici
        spec.valid = false;

    return spec;
}*/