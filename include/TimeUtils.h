//
// Created by Hemal Patel on 12/21/25.
//

#ifndef CPPSMARTFILEORGANIZER_TIMEUTILS_H
#define CPPSMARTFILEORGANIZER_TIMEUTILS_H
#include <filesystem>
#include <sstream>

inline std::time_t to_time_t(std::filesystem::file_time_type ftime) {
    // 1. Calculate the offset between the file's write time and 'now' in the file clock
    auto now_file_clock = std::filesystem::file_time_type::clock::now();
    auto duration = ftime - now_file_clock;

    // 2. Apply that same duration to the system clock's 'now'
    auto now_system_clock = std::chrono::system_clock::now();
    auto system_time_point = now_system_clock + std::chrono::duration_cast<std::chrono::system_clock::duration>(duration);

    // 3. Convert the resulting system time_point to time_t
    return std::chrono::system_clock::to_time_t(system_time_point);
}

inline std::string time_to_string(std::time_t t)
{
    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &t);   // Windows
#else
    localtime_r(&t, &tm);   // POSIX
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

#endif //CPPSMARTFILEORGANIZER_TIMEUTILS_H