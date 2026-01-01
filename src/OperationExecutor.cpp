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
    return false;
}

bool OperationExecutor::executeAllOperations(std::vector<FileOperation>& operations) {
    return false;
}

bool OperationExecutor::undoLast() {
    return false;
}

bool OperationExecutor::undoAll() {
    return false;
}


