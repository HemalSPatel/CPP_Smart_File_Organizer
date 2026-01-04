//
// Created by Hemal Patel on 1/1/26.
//
#include <filesystem>

#include "../include//OperationExecutor.h"

#include <iostream>

namespace fs = std::filesystem;

bool OperationExecutor::executeOperation(FileOperation& operation) {
    try {
        if (!exists(operation.sourcePath)) {
            std::cerr << "File " << operation.sourcePath << " does not exist." << std::endl;
            return false;
        }

        if (!exists(operation.destinationPath.parent_path())) {
            // Creating a new directory for the source if it does not exist already
            fs::create_directories(operation.destinationPath.parent_path());
        }
        fs::rename(operation.sourcePath, operation.destinationPath);
        operation.completed = true;
        completedOperations.push(operation);
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cerr << "Error moving file " << operation.sourcePath << std::endl;
    return false;
}

bool OperationExecutor::executeAllOperations(std::vector<FileOperation>& operations) {
    int counter = 0;
    int skipCounter = 0;
    for (auto& operation : operations)
    {
        bool curr = executeOperation(operation);
        if (!curr) {
            std::cout << "A problem occurred while moving files only " << counter << " were moved." << std::endl;
            // can ask the user if they want to continue with the rest of the files (skipping the current), undo the past actions, or just leave it as is stopping at the file that made the error
            // can replace this with frontend logic when that is created but for now this is fine
            bool running = true;
            bool skipFile = false;
            while (running) {
                char command;
                std::cout << "Options: (c)ontinue, (s)top, (u)ndo all and stop: ";
                std::cin >> command;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                switch (command)
                {
                    case 'c':
                        running = false;
                        skipFile = true;
                        break;
                    case 's':
                        running = false;
                        break;
                    case 'u':
                        running = false;
                        undoAll();
                        break;
                    default:
                        std::cout << "Invalid command." << std::endl;
                }
            }

            if (skipFile) {
                skipCounter++;
                continue;
            }
            return false;
        }
        counter ++;
    }
    std::cout << "Successfully moved " << counter << " files." << std::endl;
    if (skipCounter > 0) std::cout << "Skipped " << skipCounter << " files." << std::endl;
    return true;
}

bool OperationExecutor::undoLast() {
    if (completedOperations.empty())
    {
        std::cerr << "Undo operation called with no completed operations." << std::endl;
        return false;
    }
    FileOperation lastOperation = completedOperations.top();

    try {
        if (!fs::exists(lastOperation.sourcePath.parent_path())) {
            // Creating the old directory if it does not exist anymore
            fs::create_directories(lastOperation.sourcePath.parent_path());
        }
        fs::rename(lastOperation.destinationPath, lastOperation.sourcePath);
        std::cout << "Moved: " << lastOperation.destinationPath.filename() << " back to " << lastOperation.sourcePath.parent_path() << std::endl; // Here for testing

        if (fs::is_empty(lastOperation.destinationPath.parent_path())) {
            std::cout << "Removing empty directory: " << lastOperation.destinationPath.parent_path() << std::endl;
            fs::remove(lastOperation.destinationPath.parent_path());
        }
        completedOperations.pop();
        return true;
    } catch ( const std::filesystem::filesystem_error& e )
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool OperationExecutor::undoAll() {
    // For now this is going to undo all of the past file moves on the stack until I implement batch-specific undo's
    while (completedOperations.size() > 0)
    {
        bool curr = undoLast();
        if (!curr) {
            return false;
        }
    }
    return true;
}


