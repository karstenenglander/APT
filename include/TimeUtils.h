#pragma once
#include <string>
#include <filesystem>

class TimeUtils {
public:
    static std::string fileTimeToSQLiteString(std::filesystem::file_time_type ftime);
};