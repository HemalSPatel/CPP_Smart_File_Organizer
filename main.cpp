#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

#include "include/FileEntry.hpp"

namespace fs = std::filesystem;
using namespace std::chrono;

std::string human_readable_size(std::uintmax_t bytes)
{
    const char* suffixes[] = { "B", "KB", "MB", "GB", "TB", "PB" };
    size_t suffix_index = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && suffix_index < 5) {
        size /= 1024;
        ++suffix_index;
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << size << " " << suffixes[suffix_index];
    return out.str();
}

std::time_t to_time_t(fs::file_time_type ftime) {
    // 1. Calculate the offset between the file's write time and 'now' in the file clock
    auto now_file_clock = fs::file_time_type::clock::now();
    auto duration = ftime - now_file_clock;

    // 2. Apply that same duration to the system clock's 'now'
    auto now_system_clock = std::chrono::system_clock::now();
    auto system_time_point = now_system_clock + std::chrono::duration_cast<std::chrono::system_clock::duration>(duration);

    // 3. Convert the resulting system time_point to time_t
    return std::chrono::system_clock::to_time_t(system_time_point);
}

std::string time_to_string(std::time_t t)
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

int main() {
    //fs::path start_path = "/Users/hemal/Desktop/Niyu Pics";
    fs::path start_path = "/Users/hemal/Desktop/Neetcode";
    std::error_code ec;
    std::cout << "Displaying contents of: " << (start_path) << std::endl;

    std::vector<FileEntry> files;

    try {
        if (!fs::exists(start_path))
        {
            std::cerr << "Path " << start_path << " does not exist." << std::endl;
            return 1;
        }
        if (!fs::is_directory(start_path))
        {
            std::cerr << "Path " << start_path << " is not a directory." << std::endl;
            return 1;
        }
        for (const auto& entry : fs::recursive_directory_iterator(start_path, ec)) {
            if (ec)
            {
                std::cerr << "Error accessing: " << entry.path().string() << " - " << ec.message() << std::endl;
                ec.clear();
                continue;
            }
            const auto& path = entry.path();
            if (!is_directory(path))
            {
                FileEntry file;
                file.fileSize = fs::file_size(path);
                file.filePath = path;

                try {
                    // Convert to std::time_t using the portable helper
                    std::time_t tt = to_time_t(fs::last_write_time(path));
                    file.lastModified = tt;
                    // Convert std::time_t to a local time string for display
                    //std::cout << "File write time (time_t): " << std::asctime(std::localtime(&tt)) << std::endl;

                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Filesystem error (time conversion): " << e.what() << std::endl;
                    continue;
                }

                files.push_back(file);
            }

            //std::cout << (fs::is_directory(path) ? "[DIR]" : "[FILE]") << " ";
            //std::cout << path.parent_path().filename() << " - " << path.stem() << " | " << path.extension() <<  std::endl;
        }
        std::cout << "Collected " << files.size() << " file entries." <<std::endl;

        for ( int i = 0; i < 3; i++ ) {
            // printing the first 3 files
            std::cout << files[i].filePath << " | ";
            std::cout << human_readable_size(files[i].fileSize) << " | ";
            std::cout << time_to_string(files[i].lastModified) << std::endl;
        }

        if (ec) {
            std::cerr << "An error occurred during final iteration: " << ec.message() << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return 0;
}