//
// Created by Hemal Patel on 12/21/25.
//

#ifndef CPPSMARTFILEORGANIZER_FILEORGANIZER_H
#define CPPSMARTFILEORGANIZER_FILEORGANIZER_H
#include <vector>
#include <unordered_map>
#include <string>

#include "FileEntry.h"
#include "FileOperation.h"

struct OrganizeOptions {
    bool includeHidden = false;
    bool includeSystem = false;
    // Easy to add more later:
    // bool skipEmpty = true;
    // std::uintmax_t minSize = 0;
};

enum class sortType {
    Category,
    Extension,
};

class FileOrganizer {
public:
    explicit FileOrganizer(const std::vector<FileEntry>& files): files(files) {}
    //File grouping functions
    std::unordered_map<std::string, std::vector<FileEntry>> groupByExtension();
    std::unordered_map<std::string, std::vector<FileEntry>> groupByCategory();
    // File planning functions
    std::vector<FileOperation> planOperations(const std::filesystem::path& baseDirectory, sortType type, const OrganizeOptions& options = {});
private:
    std::vector<FileEntry> files;
    static std::string normalizeExtension(const std::filesystem::path& path);
    std::unordered_map<std::string, std::string> extensionToCategory = {
        // Documents
        {".pdf", "Documents"}, {".doc", "Documents"}, {".docx", "Documents"},
        {".txt", "Documents"}, {".rtf", "Documents"}, {".odt", "Documents"},
        {".xls", "Documents"}, {".xlsx", "Documents"}, {".ppt", "Documents"}, {".pptx", "Documents"},

        // Images
        {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"},
        {".gif", "Images"}, {".bmp", "Images"}, {".svg", "Images"}, {".webp", "Images"},

        // Audio
        {".mp3", "Audio"}, {".wav", "Audio"}, {".flac", "Audio"},
        {".aac", "Audio"}, {".ogg", "Audio"}, {".m4a", "Audio"},

        // Video
        {".mp4", "Video"}, {".mov", "Video"}, {".avi", "Video"},
        {".mkv", "Video"}, {".wmv", "Video"}, {".flv", "Video"},

        // Code
        {".cpp", "Code"}, {".h", "Code"}, {".hpp", "Code"}, {".py", "Code"},
        {".js", "Code"}, {".java", "Code"}, {".c", "Code"}, {".cs", "Code"},
        {".html", "Code"}, {".css", "Code"},

        // Archives
        {".zip", "Archives"}, {".tar", "Archives"}, {".gz", "Archives"},
        {".rar", "Archives"}, {".7z", "Archives"}
    };
};


#endif //CPPSMARTFILEORGANIZER_FILEORGANIZER_H