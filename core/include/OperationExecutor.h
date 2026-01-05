//
// Created by Hemal Patel on 12/31/25.
//

#ifndef CPPSMARTFILEORGANIZER_OPERATIONEXECUTOR_H
#define CPPSMARTFILEORGANIZER_OPERATIONEXECUTOR_H

#include <vector>
#include <stack>

#include "FileOperation.h"


class OperationExecutor
{
public:
    bool executeOperation(FileOperation& operation);
    bool executeAllOperations(std::vector<FileOperation>& operations);
    bool undoLast();
    bool undoAll();

private:
    std::stack<FileOperation> completedOperations;
};

#endif //CPPSMARTFILEORGANIZER_OPERATIONEXECUTOR_H