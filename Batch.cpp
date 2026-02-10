#include "Batch.h"
#include "Parser.h"
#include "CommandFactory.h"
#include "CommandIOStream.h"
#include "ConsoleIn.h"
#include "ConsoleOut.h"
#include "FileIn.h"
#include "FileOut.h"
#include "TextIn.h"
#include "InStream.h"
#include "OutStream.h"
#include "Interpreter.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string removeFirstOccurrenceBatch(const string& str, const string& word) {
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

void BatchCommand::execute(InStream& in, OutStream& output)
{

    string line;
    string source = in.read();
    stringstream ss(source);

    while (getline(ss, line)) {
        if (line.empty()) continue;
        // cout << line << "\n";

        Parser parser;
        CommandFactory factory;
        string name;
        CommandIOStream stream;
        vector<string> inOut;
        bool redIn = false;
        bool redOut = false;
        bool noIn = false;
        bool noOut = false;
        bool doubleRedOut = false;
        string tempLine;
        int redOutCounter = 0;

        CommandSpec spec = parser.parse(line);

        if (!spec.valid) {
            output.write("Uncorrect call of batch\n"); // ovde treba cout
            return;
        }

        if (count(line.begin(), line.end(), '>') > 2) {
            cout << "Error 2 redirections" << "\n";
            continue;
        }
        else if (count(line.begin(), line.end(), '>') == 2) {
            doubleRedOut = true;
        }
        string wordRemove = "<" + spec.redIn;
        name = spec.name;
        if (!spec.redIn.empty()) redIn = true;
        if (!spec.redOut.empty()) redOut = true;
        tempLine = line;
        if (redIn) {
            line = removeFirstOccurrenceBatch(line, wordRemove);
        }
        if (tempLine == line && redIn) {
            line = removeFirstOccurrenceBatch(line, "<");
            line = removeFirstOccurrenceBatch(line, spec.redIn);
        }
        wordRemove = ">" + spec.redOut;
        tempLine = line;
        if (redOut) {
            line = removeFirstOccurrenceBatch(line, wordRemove);
            // ++redOutCounter;
        }
        if (tempLine == line && redOut) {
            line = removeFirstOccurrenceBatch(line, ">");
            line = removeFirstOccurrenceBatch(line, spec.redOut);
            // redOutCounter++;
        }
        wordRemove = ">>" + spec.redOut;
        tempLine = line;
        if (redOut) {
            line = removeFirstOccurrenceBatch(line, wordRemove);
            // redOutCounter++;
            // doubleRedOut = true;
        }
        if (tempLine == line && redOut) {
            line = removeFirstOccurrenceBatch(line, ">>");
            line = removeFirstOccurrenceBatch(line, spec.redOut);
            // redOutCounter++;
        }
        if (redOutCounter > 1) {
            cout << "counter: Uncorrect input" << '\n';
            continue;
        }
        
        inOut = stream.choseStream(name, line, redIn, redOut);
        if (inOut.empty()) {
            output.write("Uncorrect input\n");
            continue;
        }
        unique_ptr<InStream> in;
        unique_ptr<OutStream> out;

        // svaki put kada se ocekuje console input, stavi kao text sledece linije

        if (inOut[1] == "flagIn") {
            if (inOut[0] == "console") {
                // getline(ss, line);
                // const std::string& constLine = line;
                in = make_unique<ConsoleIn>(); // ovde cita sledecu liniju
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
            in = make_unique<ConsoleIn>(); // ovo se nece koristiti
        }


        auto command = factory.create(spec);
        if (command) {
            command->execute(*in, *out);
        }
        else {
            output.write("Unknown command: \n");
        }
    }
}
