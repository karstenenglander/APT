#include "TimeUtils.h"
#include <chrono>
#include <ctime>
#include <sstream>

std::string TimeUtils::fileTimeToSQLiteString(std::filesystem::file_time_type ftime) {
    if (ftime == std::filesystem::file_time_type::min()) {
        return "";
    }

    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>
        (ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

    struct tm tmbuf;
    if (localtime_s(&tmbuf, &cftime) != 0) {
        return "";
    }

    char buffer[20];
    std::strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", &tmbuf);
    return std::string(buffer);
}