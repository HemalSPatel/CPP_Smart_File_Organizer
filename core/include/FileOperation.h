//
// Created by Hemal Patel on 12/24/25.
//

#ifndef CPPSMARTFILEORGANIZER_FILEOPERATION_H
#define CPPSMARTFILEORGANIZER_FILEOPERATION_H
#include <filesystem>

struct FileOperation {
    std::filesystem::path sourcePath;
    std::filesystem::path destinationPath;
    std::filesystem::path baseDirectory;
    std::time_t timestamp;
    bool completed;
};

#endif //CPPSMARTFILEORGANIZER_FILEOPERATION_H