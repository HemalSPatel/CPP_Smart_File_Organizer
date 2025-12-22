//
// Created by Hemal Patel on 12/21/25.
//

#ifndef CPPSMARTFILEORGANIZER_FILEORGANIZER_H
#define CPPSMARTFILEORGANIZER_FILEORGANIZER_H
#include <vector>
#include <unordered_map>
#include <string>

#include "FileEntry.h"


class FileOrganizer {
public:
    explicit FileOrganizer(const std::vector<FileEntry>& files): files(files) {}
    std::unordered_map<std::string, std::vector<FileEntry>> groupByExtension();
    std::unordered_map<std::string, std::vector<FileEntry>> groupByCategory();
private:
    std::vector<FileEntry> files;
    static std::string normalizeExtension(const std::filesystem::path& path);
};


#endif //CPPSMARTFILEORGANIZER_FILEORGANIZER_H