#include "Date.h"
#include <chrono>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

void DateCommand::execute(InStream& in, OutStream& out)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&now);

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);

    out.write(std::string(buffer));
}
