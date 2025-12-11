#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    //fs::path start_path = "/Users/hemal/Desktop/Niyu Pics";
    fs::path start_path = "/Users/hemal/Desktop/Neetcode";

    std::cout << "Displaying contents of: " << (start_path) << std::endl;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(start_path)) {
            const auto& path = entry.path();
            std::cout << (is_directory(path) ? "[DIR]" : "[FILE]") << " ";
            std::cout << path.parent_path().filename() << " - " << path.filename() <<  std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return 0;
}