#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    //fs::path start_path = "/Users/hemal/Desktop/Niyu Pics";
    fs::path start_path = "/Users/hemal/Desktop/Neetcode";
    std::error_code ec;
    std::cout << "Displaying contents of: " << (start_path) << std::endl;

    try {
        if (!fs::exists(start_path))
        {
            std::cerr << "Path " << start_path << " does not exist." << std::endl;
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

            std::cout << (fs::is_directory(path) ? "[DIR]" : "[FILE]") << " ";
            std::cout << path.parent_path().filename() << " - " << path.stem() << path.extension() <<  std::endl;
        }
        if (ec) {
            std::cerr << "An error occurred during final iteration: " << ec.message() << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return 0;
}