//
// Created by Hemal Patel on 12/21/25.
//

#ifndef CPPSMARTFILEORGANIZER_FORMATUTILS_H
#define CPPSMARTFILEORGANIZER_FORMATUTILS_H
#include <sstream>
#include <iomanip>
#include <string>

namespace FormatUtils {
    inline std::string human_readable_size(std::uintmax_t bytes)
    {
        const char* suffixes[] = { "B", "KB", "MB", "GB", "TB", "PB" };
        size_t suffix_index = 0;
        auto size = static_cast<double>(bytes);

        while (size >= 1024 && suffix_index < 5) {
            size /= 1024;
            ++suffix_index;
        }

        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << size << " " << suffixes[suffix_index];
        return out.str();
    }
    inline std::string padNumber(int num) {
        std::ostringstream oss;
        // Set the fill character to '0'
        oss << std::setfill('0');
        // Set the width of the output field to 2
        oss << std::setw(2);
        // Insert the number
        oss << num;
        // Return the resulting string
        return oss.str();
    }
}

#endif //CPPSMARTFILEORGANIZER_FORMATUTILS_H