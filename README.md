# Smart File Organizer

A cross-platform CLI tool built in C++17 that automatically organizes messy directories by file type, category, or customizable rules. Perfect for cleaning up Downloads folders, sorting photos, or classifying documents.

## Features

- **Category-Based Organization**: Automatically sorts files into semantic categories (Documents, Images, Audio, Video, Code, Archives)
- **Extension-Based Organization**: Groups files by their file extensions
- **Safe Operations**: Preview changes before executing, with full undo support
- **Collision Detection**: Warns about duplicate filenames before moving
- **Hidden/System File Filtering**: Optionally skip hidden files (`.dotfiles`) and system files (`.DS_Store`, `Thumbs.db`)
- **Cross-Platform**: Works on macOS, Linux, and Windows
- **Interactive Error Handling**: Choose to continue, stop, or undo when errors occur

## Project Structure

```
CppSmartFileOrganizer/
├── include/
│   ├── FileEntry.h          # File metadata structure
│   ├── FileOperation.h      # Move operation record
│   ├── FileOrganizer.h      # Grouping and planning logic
│   ├── OperationExecutor.h  # Execution and undo logic
│   ├── FormatUtils.h        # Size formatting utilities
│   └── TimeUtils.h          # Timestamp utilities
├── src/
│   ├── FileOrganizer.cpp
│   └── OperationExecutor.cpp
├── main.cpp
├── CMakeLists.txt
└── README.md
```

## Requirements

- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2017+)
- CMake 3.10+

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Basic Usage

```cpp
#include "FileOrganizer.h"
#include "OperationExecutor.h"

// 1. Collect files from a directory
std::vector<FileEntry> files;
for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
    if (!fs::is_directory(entry.path())) {
        FileEntry file;
        file.filePath = entry.path();
        file.fileSize = fs::file_size(entry.path());
        file.lastModified = /* ... */;
        files.push_back(file);
    }
}

// 2. Plan operations
FileOrganizer organizer(files);
auto operations = organizer.planOperations(targetPath, sortType::Category);

// 3. Preview (optional)
for (const auto& op : operations) {
    std::cout << op.sourcePath << " -> " << op.destinationPath << std::endl;
}

// 4. Execute
OperationExecutor executor;
executor.executeAllOperations(operations);

// 5. Undo if needed
executor.undoAll();
```

### Organization Options

```cpp
// Filter hidden and system files (default behavior)
OrganizeOptions options;
options.includeHidden = false;  // Skip .dotfiles
options.includeSystem = false;  // Skip .DS_Store, Thumbs.db, etc.

auto operations = organizer.planOperations(path, sortType::Category, options);

// Or include everything
OrganizeOptions includeAll{true, true};
auto allOperations = organizer.planOperations(path, sortType::Category, includeAll);
```

### Sort Types

- `sortType::Category` - Groups files into semantic folders:
    - Documents: `.pdf`, `.doc`, `.docx`, `.txt`, `.rtf`, `.odt`, `.xls`, `.xlsx`, `.ppt`, `.pptx`
    - Images: `.jpg`, `.jpeg`, `.png`, `.gif`, `.bmp`, `.svg`, `.webp`
    - Audio: `.mp3`, `.wav`, `.flac`, `.aac`, `.ogg`, `.m4a`
    - Video: `.mp4`, `.mov`, `.avi`, `.mkv`, `.wmv`, `.flv`
    - Code: `.cpp`, `.h`, `.hpp`, `.py`, `.js`, `.java`, `.c`, `.cs`, `.html`, `.css`
    - Archives: `.zip`, `.tar`, `.gz`, `.rar`, `.7z`
    - Other: Everything else

- `sortType::Extension` - Groups files by their literal extension (`.jpg`, `.pdf`, etc.)

## Error Handling

When an error occurs during execution, the tool prompts:
```
A problem occurred while moving files only 5 were moved.
Options: (c)ontinue, (s)top, (u)ndo all and stop:
```

- **c** - Skip the problematic file and continue with remaining files
- **s** - Stop execution, keeping changes made so far
- **u** - Undo all completed operations and stop

## Example Output

```
Displaying contents of: "/Users/user/Downloads"
Collected 47 file entries.

Planned operations:
"report.pdf" -> "/Users/user/Downloads/Documents/report.pdf"
"photo.jpg" -> "/Users/user/Downloads/Images/photo.jpg"
"song.mp3" -> "/Users/user/Downloads/Audio/song.mp3"

Executing...
Successfully moved 47 files.

Press enter to undo...
Moved: "song.mp3" back to "/Users/user/Downloads"
Moved: "photo.jpg" back to "/Users/user/Downloads"
Moved: "report.pdf" back to "/Users/user/Downloads"
Removing empty directory: "/Users/user/Downloads/Audio"
Removing empty directory: "/Users/user/Downloads/Images"
Removing empty directory: "/Users/user/Downloads/Documents"
```

## Roadmap

- [x] Directory traversal with `std::filesystem`
- [x] File metadata extraction
- [x] Organization by extension
- [x] Organization by category
- [x] Safe file operations with error handling
- [x] Full undo functionality
- [x] Hidden/system file filtering
- [x] Collision detection
- [ ] CLI argument parsing
- [ ] Dry-run mode
- [ ] Operation logging to file
- [ ] Date-based organization
- [ ] Regex pattern matching
- [ ] Config file support (JSON)
- [ ] Duplicate file detection via hashing
- [ ] Real-time directory watching
- [ ] LLM-based smart categorization
- [ ] Web/Native UI frontend

