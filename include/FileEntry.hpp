//
// Created by Hemal Patel on 12/18/25.
//

#ifndef CPPSMARTFILEORGANIZER_FILEENTRY_H
#define CPPSMARTFILEORGANIZER_FILEENTRY_H
#include <filesystem>


struct FileEntry
{
    std::filesystem::path filePath;
    std::time_t lastModified;
    std::uintmax_t fileSize;
};


#endif //CPPSMARTFILEORGANIZER_FILEENTRY_H