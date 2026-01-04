#include <iostream>
#include <filesystem>
#include <vector>

#include "include/FileEntry.h"
#include "include/FormatUtils.h"
#include "include/TimeUtils.h"
#include "include/FileOrganizer.h"
#include "include/OperationExecutor.h"

namespace fs = std::filesystem;
using namespace std::chrono;

int main() {
    //fs::path start_path = "/Users/hemal/Desktop/Niyu Pics";
    // fs::path start_path = "/Users/hemal/Desktop/Neetcode";
    fs::path start_path = "/Users/hemal/Desktop/TestOrganizer";
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
                    std::time_t tt = TimeUtils::to_time_t(fs::last_write_time(path));
                    file.lastModified = tt;
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

        const size_t limit = std::min(static_cast<size_t>(3), files.size());

        for (size_t i = 0; i < limit; i++ ) {
            std::cout << files[i].filePath << " | ";
            std::cout << FormatUtils::human_readable_size(files[i].fileSize) << " | ";
            std::cout << TimeUtils::time_to_string(files[i].lastModified) << std::endl;
        }

        FileOrganizer fileOrganizer(files);
        std::unordered_map<std::string, std::vector<FileEntry>> organizedFiles = fileOrganizer.groupByExtension();
        for (const auto& [key, value] : organizedFiles) {
            std::cout << key << ": " << value.size() << " files" << std::endl;
        }

        auto categorized = fileOrganizer.groupByCategory();
        for (const auto& [category, categoryFiles] : categorized) {
            std::cout << category << ": " << categoryFiles.size() << " files" << std::endl;
        }

        auto operations = fileOrganizer.planOperations(start_path, sortType::Category);
        std::cout << "\nPlanned operations:" << std::endl;
        for (const auto& op : operations) {
            std::cout << op.sourcePath << " -> " << op.destinationPath << std::endl;
        }

        std::cout << "\nExecuting..." << std::endl;
        OperationExecutor executor;
        executor.executeAllOperations(operations);

        std::cout << "\nPress enter to undo...";
        std::cin.get();

        executor.undoAll();

        if (ec) {
            std::cerr << "An error occurred during final iteration: " << ec.message() << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return 0;
}