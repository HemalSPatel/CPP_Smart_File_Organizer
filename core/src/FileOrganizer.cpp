//
// Created by Hemal Patel on 12/21/25.
//
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_set>

#include "FileEntry.h"
#include "FileOrganizer.h"
#include "FileOperation.h"
#include "FormatUtils.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <sys/stat.h>
#include <unistd.h>
#endif

std::string FileOrganizer::buildDatePath(std::time_t timestamp, DateGranularity granularity) {
    std::tm tm{};
    #ifdef _WIN32
        localtime_s(&tm, &timestamp); // Windows
    #else
        localtime_r(&timestamp, &tm); // POSIX
    #endif


    std::string year = std::to_string((tm.tm_year + 1900)); // tm_year is years since 1900
    int month = (tm.tm_mon + 1);    // tm_mon is 0-11 (add 1 for 1-12)
    int day = (tm.tm_mday);        // tm_mday is day of the month (1-31)

    switch (granularity)
    {
        case DateGranularity::Day:
            return year + "/" + FormatUtils::padNumber(month) + "/" + FormatUtils::padNumber(day);
        case DateGranularity::Month:
            return year + "/" + FormatUtils::padNumber(month);
        case DateGranularity::Year:
            return year;
        default:
            std::cerr << "Unknown DateGranularity Given." << std::endl;
            return "";
    }
}

bool isHidden(const std::filesystem::path& p) {
    std::string filename = p.filename().string();
    // 1. Check for Unix-style hidden files (starts with '.')
    // This applies to Linux, macOS, and also many cross-platform apps on Windows
    if (filename.length() > 1 && filename[0] == '.') {
        // Also ensure it's not the special "." or ".." directories
        if (filename != "." && filename != "..") {
            return true;
        }
    }
    // 2. Check for Windows-style hidden files (attribute check)
    #ifdef _WIN32
    // Use the Windows API to get file attributes
    DWORD attributes = GetFileAttributesA(p.string().c_str());
    if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_HIDDEN)) {
        return true;
    }
    #endif

    // If none of the above conditions met, the file is not considered hidden by system conventions
    return false;
}

bool isSystemFile(const std::filesystem::path& p) {
    // First, check if the file exists using standard C++17 filesystem
    if (!std::filesystem::exists(p)) {
        return false;
    }

    // Platform-specific checks
    #ifdef _WIN32
    // On Windows, use GetFileAttributesW to check for the SYSTEM attribute.
    // The SYSTEM attribute indicates a system file.
    DWORD attributes = GetFileAttributesW(p.c_str());
    if (attributes != INVALID_FILE_ATTRIBUTES) {
        return (attributes & FILE_ATTRIBUTE_SYSTEM) != 0;
    }
    // If GetFileAttributesW fails, we can't determine the status.
    return false;

    #elif defined(__APPLE__) || defined(__linux__)
    std::string pathStr = p.string();
    if (pathStr.rfind("/bin/", 0) == 0 ||
        pathStr.rfind("/sbin/", 0) == 0 ||
        pathStr.rfind("/usr/bin/", 0) == 0 ||
        pathStr.rfind("/usr/sbin/", 0) == 0 ||
        pathStr.rfind("/etc/", 0) == 0 ||
        pathStr.rfind("/dev/", 0) == 0) {
        return true;
    }

    // There isn't a direct "system file" attribute like on Windows in POSIX,
    // so this is the best we can do with cross-platform C++ and POSIX APIs.
    return false;

    #else
    // Fallback for other platforms where specific checks are not implemented
    return false;
    #endif
}

static bool shouldSkipFile(const FileEntry& file, const OrganizeOptions& options) {
    std::string filename = file.filePath.filename().string();

    if (!options.includeHidden) {
        if (isHidden(file.filePath)) {
            return true;
        }
    }
    if (!options.includeSystem) {
        if (isSystemFile(file.filePath)) {
            return true;
        }
    }
    return false;
}

/**
 * Converts a string to lowercase.
 */
std::string to_lower(std::string str) {  // Takes by value, returns modified copy
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return str;
}

std::string FileOrganizer::normalizeExtension(const std::filesystem::path& path) {
    std::string ext = path.extension().string();
    return to_lower(ext);
};

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByExtension() {
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    for (const FileEntry& file : files)
    {
        std::string ext = normalizeExtension(file.filePath);
        // Have to make sure that the folders do not have the "." at the beginning so the folders are not hidden
        if (ext.length() > 1 && ext[0] == '.') {
            ext = ext.substr(1, ext.size());
        }
        result[ext].push_back(file);
    }
    return result;
}

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByCategory() {
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    for (const FileEntry& file : files) {
        std::string ext = normalizeExtension(file.filePath);

        auto it = extensionToCategory.find(ext);
        if (it != extensionToCategory.end()) {
            result[it->second].push_back(file);
        } else {
            result["Other"].push_back(file);
        }
    }
    return result;
}

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByDate(DateGranularity granularity) {
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    for (const FileEntry& file : files) {
        std::string datePath = buildDatePath(file.lastModified, granularity);
        result[datePath].push_back(file);
    }
    return result;
}

std::vector<FileOperation> FileOrganizer::planOperations(const std::filesystem::path& baseDirectory, sortType type, const OrganizeOptions& options) {
    std::vector<FileOperation> result;
    std::unordered_map<std::string, std::vector<FileEntry>> groupedFiles;
    std::unordered_set<std::string> destinationPaths;

    switch (type) {
        case sortType::Category:
            groupedFiles = groupByCategory();
            break;
        case sortType::Extension:
            groupedFiles = groupByExtension();
            break;
        case sortType::Date:
            groupedFiles = groupByDate(options.granularity);
            break;
        default:
            std::cerr << "Invalid method specified" << std::endl;
            return {};
    }

    std::time_t currentTime_c = std::time(nullptr);
    for (const auto& [key, fileVector]: groupedFiles) {
        for (const FileEntry& file : fileVector)
        {
            if (!shouldSkipFile(file, options)) {
                FileOperation operation;
                operation.sourcePath = file.filePath;
                operation.completed = false;
                operation.timestamp = currentTime_c;
                operation.baseDirectory = baseDirectory;
                operation.destinationPath = baseDirectory / key / file.filePath.filename();
                if (destinationPaths.find(operation.destinationPath.string()) != destinationPaths.end())
                    std::clog << "WARNING: Duplicate file detected" << std::endl;
                else {
                    result.push_back(operation);
                }
                destinationPaths.insert(operation.destinationPath.string());
            }else
            {
                std::cout << "File Skipped: " << file.filePath.filename() << std::endl;
            }
        }
    }
    return result;
}

