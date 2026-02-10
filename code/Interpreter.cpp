#include "Interpreter.h"
#include "Parser.h"
#include "Command.h"
#include "CommandFactory.h"
#include "CommandIOStream.h"
#include "ConsoleIn.h"
#include "ConsoleOut.h"
#include "FileIn.h"
#include "FileOut.h"
#include "TextIn.h"
#include "InStream.h"
#include "OutStream.h"
#include "PipeIn.h"
#include "PipeOut.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>

using namespace std;

string globalPrompt = "$";

string removeFirstOccurrence(const string& str, const string& word) {
    stringstream ss(str);
    string token;
    string result;
    bool removed = false;
    bool first = true;

    while (ss >> token) {
        if (!removed && token == word) {
            removed = true; // skip this token once
            continue;
        }
        if (!first) result += " ";
        result += token;
        first = false;
    }

    return result;
}

string removeLastOccurrence(const string& str, const string& word) {
    stringstream ss(str);
    string token;
    vector<string> tokens;

    // Split the string into tokens
    while (ss >> token) {
        tokens.push_back(token);
    }

    // Find the last occurrence of `word`
    for (int i = tokens.size() - 1; i >= 0; --i) {
        if (tokens[i] == word) {
            tokens.erase(tokens.begin() + i);
            break; // remove only the last occurrence
        }
    }

    // Reconstruct the string
    string result;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0) result += " ";
        result += tokens[i];
    }

    return result;
}

vector<string> pipeCheck(const string& line, bool& isPipe)
{
    vector<string> parts;
    if (line.find("|") != string::npos) {

        stringstream ss(line);
        string part;

        while (getline(ss, part, '|')) {

            size_t start = part.find_first_not_of(" \t");
            if (start != string::npos) {
                part = part.substr(start);
            }
            else {
                part.clear(); // only spaces
            }

            parts.push_back(part);
        }

        isPipe = true;
        // return parts;
    }
    return parts;
}

void Interpreter::run() {

    string line;
    bool isPipe = false;
    vector<string> parts;

    while (true) {
        if (!isPipe) {
            cout << globalPrompt << " ";
            if (!getline(cin, line)) break;
            if (line.empty()) continue;
            parts = pipeCheck(line, isPipe);
            if (isPipe) continue;
        }
        else {
            unique_ptr<InStream> in;
            for (int i = 0; i < parts.size(); i++) {
                // cout << parts[i] << "\n";
                Parser parser;
                bool doubleRedOut = false;
                bool redIn = false;
                bool redOut = false;
                string tempLine;
                CommandIOStream stream;
                vector<string> inOut;
                CommandFactory factory;
                bool redOutCounter = false;

                line = parts[i];
                // cout << line << "\n";
                
                // redirection check > >>
                if (count(line.begin(), line.end(), '>') > 2) {
                    cout << "Error 2 redirections" << "\n";
                    continue;
                }
                else if (count(line.begin(), line.end(), '>') == 2) {
                    doubleRedOut = true;
                }
                CommandSpec spec = parser.parse(line);
                string name = spec.name;

                // unique_ptr<InStream> in;
                unique_ptr<OutStream> out;

                if (i == 0) {
                    if (!spec.redIn.empty()) redIn = true;
                    // cout << redIn << "\n";
                    if (!spec.redOut.empty()) {
                        cout << "No redirection 'out' in first pipe element" << "\n";
                        break;
                    }
                    string wordRemove = "<" + spec.redIn;
                    tempLine = line;
                    if (redIn) {
                        line = removeFirstOccurrence(line, wordRemove);
                    }
                    // if (tempLine != line) cout << "jeste" << "\n";
                    if (count(line.begin(), line.end(), '<') == 1 && redIn) {
                        line = removeFirstOccurrence(line, "<");
                        line = removeFirstOccurrence(line, spec.redIn);
                    }
                    // cout << line << "\n";
                    inOut = stream.choseStream(name, line, redIn, redOut);
                    if (inOut.empty()) {
                        cout << "Uncorrect input" << '\n';
                        continue;
                    }
                    /*
                    unique_ptr<InStream> in;
                    unique_ptr<OutStream> out;
                    */
                    if (inOut[1] == "flagIn") {
                        if (inOut[0] == "console") {
                            in = make_unique<ConsoleIn>();
                        }
                        else if (inOut[0] == "file") {
                            if (!spec.fileName.empty()) {
                                in = make_unique<FileIn>(spec.fileName);
                            }
                            else {
                                in = make_unique<FileIn>(spec.redIn);
                            }
                        }
                        else if (inOut[0] == "text") {
                            in = make_unique<TextIn>(spec.quotedArg);
                        }
                        
                    }
                    else if (inOut[1] == "flagOut") {
                        in = make_unique<ConsoleIn>();
                    }
                }
                else if (i == parts.size() - 1) {
                    if (!spec.redOut.empty()) redOut = true;
                    if (!spec.redIn.empty()) {
                        cout << "No redirection 'in' in first pipe element" << "\n";
                        break;
                    }
                    string wordRemove = ">" + spec.redOut;
                    tempLine = line;
                    if (redOut) {
                        line = removeFirstOccurrence(line, wordRemove);
                        // ++redOutCounter;
                    }
                    if (tempLine == line && redOut) {
                        line = removeFirstOccurrence(line, ">");
                        line = removeLastOccurrence(line, spec.redOut);
                        redOutCounter = true;
                    }

                    wordRemove = ">>" + spec.redOut;
                    tempLine = line;
                    if (redOut) {
                        line = removeFirstOccurrence(line, wordRemove);
                        // redOutCounter++;
                        // doubleRedOut = true;
                    }
                    if (tempLine == line && redOut) {
                        line = removeFirstOccurrence(line, ">>");
                        if (!redOutCounter)line = removeLastOccurrence(line, spec.redOut);
                        //redOutCounter++;
                    }
                    inOut = stream.choseStream(name, line, redIn, redOut);
                    if (inOut.empty()) {
                        cout << "Uncorrect input" << '\n';
                        continue;
                    }

                    if (inOut[1] == "flagIn") {
                        // in = make_unique<PipeIn>();
                        if (inOut.size() == 4) {
                            if (inOut[2] == "console") {
                                out = make_unique<ConsoleOut>();
                            }
                            else if (inOut[2] == "file") {
                                if (!spec.redOut.empty()) {
                                    out = make_unique<FileOut>(spec.redOut, doubleRedOut);
                                }
                                else {
                                    out = make_unique<FileOut>(spec.fileName, doubleRedOut);
                                }
                            }
                        }
                        else {
                            out = make_unique<ConsoleOut>();
                        }
                    }
                    else if (inOut[1] == "flagOut") {
                        if (inOut[0] == "console") {
                            out = make_unique<ConsoleOut>();
                        }
                        else if (inOut[0] == "file") {
                            if (!spec.fileName.empty()) {
                                out = make_unique<FileOut>(spec.fileName, doubleRedOut);
                            }
                            else {
                                out = make_unique<FileOut>(spec.redOut, doubleRedOut);
                            }
                        }
                        // in = make_unique<PipeIn>();
                    }

                }
                else {
                    if (!spec.redIn.empty() || !spec.redOut.empty()) {
                        cout << "Redirection is not supported" << "\n";
                        break;
                    }
                    // in = make_unique<PipeIn>();
                    // out = make_unique<PipeOut>();
                }
                auto command = factory.create(spec);
                if (command) {
                    PipeOut* tempPipe = new PipeOut();
                    if (i != parts.size() - 1) {
                        out.reset(tempPipe);
                    }
                    command->execute(*in, *out);
                    if (i != parts.size() - 1) {
                        string result = tempPipe->getContent();
                        // cout << result << "\n";
                        in = make_unique<PipeIn>(result);
                    }
                }
                else {
                    cout << "Unknown command: " << spec.name << "\n";
                }
            }
            isPipe = !isPipe;
            continue;
        }

        Parser parser;
        CommandFactory factory;
        string name;
        CommandIOStream stream;
        vector<string> inOut;
        bool redIn = false;
        bool redOut = false;
        bool doubleRedOut = false;
        string tempLine;
        bool redOutCounter = false;

        // redirection check > >>
        if (count(line.begin(), line.end(), '>') > 2) {
            cout << "Error 2 redirections" << "\n";
            continue;
        }
        else if (count(line.begin(), line.end(), '>') == 2) {
            doubleRedOut = true;
        }
        CommandSpec spec = parser.parse(line);
        name = spec.name;
        //string wordRemove = "<" + spec.redIn;
        // name = spec.name;

        if (!spec.redIn.empty()) redIn = true;
        if (!spec.redOut.empty()) redOut = true;

        string wordRemove = "<" + spec.redIn;
        tempLine = line;
        if (redIn) { 
            line = removeFirstOccurrence(line, wordRemove);
        }
        if (tempLine == line && redIn) { 
            line = removeFirstOccurrence(line, "<"); 
            line = removeFirstOccurrence(line, spec.redIn);
        }

        wordRemove = ">" + spec.redOut;
        tempLine = line;
        if (redOut) {
            line = removeFirstOccurrence(line, wordRemove);
            // ++redOutCounter;
        }
        if (tempLine == line && redOut) {
            line = removeFirstOccurrence(line, ">");
            line = removeLastOccurrence(line, spec.redOut);
            redOutCounter = true;
        }

        wordRemove = ">>" + spec.redOut;
        tempLine = line;
        if (redOut) { 
            line = removeFirstOccurrence(line, wordRemove);
            // redOutCounter++;
            // doubleRedOut = true;
        }
        if (tempLine == line && redOut) {
            line = removeFirstOccurrence(line, ">>");
            if (!redOutCounter)line = removeLastOccurrence(line, spec.redOut);
            //redOutCounter++;
        }
        if (redOutCounter > 1) {
            cout << "counter: Uncorrect input" << '\n'; 
            continue;
        }
        // cout << line << '\n';
        // cout << redOut << '\n';

        inOut = stream.choseStream(name, line, redIn, redOut);
        if (inOut.empty()) {
            cout << "Uncorrect input" << '\n';
            continue;
        }
        unique_ptr<InStream> in;
        unique_ptr<OutStream> out;

        if (inOut[1] == "flagIn") {
            if (inOut[0] == "console") {
                in = make_unique<ConsoleIn>();
            }
            else if (inOut[0] == "file") {
                if (!spec.fileName.empty()) {
                    in = make_unique<FileIn>(spec.fileName);
                }
                else {
                    in = make_unique<FileIn>(spec.redIn);
                }
            }
            else if (inOut[0] == "text") {
                in = make_unique<TextIn>(spec.quotedArg);
            }
            // out = make_unique<ConsoleOut>();
            if (inOut.size() == 4) {
                if (inOut[2] == "console") {
                    out = make_unique<ConsoleOut>();
                }
                else if (inOut[2] == "file") {
                    if (!spec.redOut.empty()) {
                        out = make_unique<FileOut>(spec.redOut, doubleRedOut);
                    }
                    else {
                        out = make_unique<FileOut>(spec.fileName, doubleRedOut);
                    }
                }
            }
            else {
                out = make_unique<ConsoleOut>();
            }
        }
        else if (inOut[1] == "flagOut"){
            if (inOut[0] == "console") {
                out = make_unique<ConsoleOut>();
            }
            else if (inOut[0] == "file") {
                if (!spec.fileName.empty()) {
                    out = make_unique<FileOut>(spec.fileName, doubleRedOut);
                }
                else {
                    out = make_unique<FileOut>(spec.redOut, doubleRedOut);
                }
            }
            in = make_unique<ConsoleIn>();
        }
        
        auto command = factory.create(spec);
        if (command) {
            command->execute(*in, *out);
        }
        else {
            cout << "Unknown command: " << spec.name << "\n";
        }
    }
}
