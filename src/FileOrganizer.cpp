//
// Created by Hemal Patel on 12/21/25.
//
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../include//FileEntry.h"
#include "../include/FileOrganizer.h"

#include <list>
#include <unordered_set>

#include "../include/FileOperation.h"

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

        if (ext.empty()) {
            std::cout << "No extension: " << file.filePath.filename() << std::endl;
        }
        result[ext].push_back(file);
    }
    return result;
}

std::unordered_map<std::string, std::vector<FileEntry>> FileOrganizer::groupByCategory() {
    std::unordered_map<std::string, std::vector<FileEntry>> result;
    for (const FileEntry& file : files)
    {
        std::string ext = normalizeExtension(file.filePath);
        if (ext.empty())
        {
            std::cout << "No extension: " << file.filePath.filename() << std::endl;
        }
        auto it = extensionToCategory.find(ext);
        if (it != extensionToCategory.end()) {
            result[it->second].push_back(file);
        } else {
            result["Other"].push_back(file);
        }
    }
    return result;
}

std::vector<FileOperation> FileOrganizer::planOperations(const std::filesystem::path& baseDirectory, sortType type) {
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
        default:
            std::cerr << "Invalid method specified" << std::endl;
            return {};
    }

    std::time_t currentTime_c = std::time(nullptr);
    for (const auto& [key, fileVector]: groupedFiles) {
        for (const FileEntry& file : fileVector)
        {
            FileOperation operation;
            operation.sourcePath = file.filePath;
            operation.completed = false;
            operation.timestamp = currentTime_c;
            operation.destinationPath = baseDirectory / key / file.filePath.filename();
            if (destinationPaths.find(operation.destinationPath.string()) != destinationPaths.end())
                std::clog << "WARNING: Duplicate file detected" << std::endl;
            else
            {
                result.push_back(operation);
            }
            destinationPaths.insert(operation.destinationPath.string());
        }
    }
    return result;
}

