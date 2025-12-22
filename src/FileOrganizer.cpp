//
// Created by Hemal Patel on 12/21/25.
//
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../include//FileEntry.h"
#include "../include/FileOrganizer.h"


/**
 * Converts a string to lowercase.
 */
std::string to_lower(std::string str) {  // Takes by value, returns modified copy
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return str;
}

std::string FileOrganizer::normalizeExtension(const std::filesystem::path& path)
{
    std::string ext = path.extension().string();
    return to_lower(ext);
};

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByExtension()
{
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    for (const FileEntry& file : files)
    {
        std::string ext = normalizeExtension(file.filePath);

        if (ext.empty()) {
            std::cout << "No extension: " << file.filePath.filename() << std::endl;
        }
        result[ext].push_back(file);
    }
    return result;
}

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByCategory()
{
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    return result;
}

