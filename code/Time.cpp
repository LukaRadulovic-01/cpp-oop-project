#include "Time.h"
#include <chrono>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

void TimeCommand::execute(InStream& in, OutStream& out) {
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm tm = *localtime(&now);

    char buffer[16];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);

    out.write(string(buffer));
}
