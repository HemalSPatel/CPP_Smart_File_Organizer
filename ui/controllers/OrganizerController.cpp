//
// Created by Hemal Patel on 1/6/26.
//

#include "OrganizerController.h"
#include "TimeUtils.h"
#include "FileOrganizer.h"
#include <QDir>
#include <filesystem>

namespace fs = std::filesystem;

OrganizerController::OrganizerController(QObject *parent)
    : QObject(parent)
{
}

void OrganizerController::setDirectoryPath(const QString &path)
{
    if (m_directoryPath != path) {
        m_directoryPath = path;
        emit directoryPathChanged();

        // Reset state when directory changes
        m_files.clear();
        m_operations.clear();
        m_filesFound.clear();
        m_plannedOperations.clear();
        m_hasExecuted = false;

        emit filesFoundChanged();
        emit plannedOperationsChanged();
        emit fileCountChanged();
        emit operationCountChanged();
        emit canExecuteChanged();
        emit canUndoChanged();
    }
}

void OrganizerController::setDirectoryFromUrl(const QUrl &url)
{
    setDirectoryPath(url.toLocalFile());
}

void OrganizerController::setStatus(const QString &status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void OrganizerController::collectFiles(const fs::path &path)
{
    m_files.clear();
    m_filesFound.clear();

    try {
        for (const auto &entry : fs::recursive_directory_iterator(path)) {
            if (!fs::is_directory(entry.path())) {
                FileEntry file;
                file.filePath = entry.path();
                file.fileSize = fs::file_size(entry.path());
                file.lastModified = TimeUtils::to_time_t(fs::last_write_time(entry.path()));
                m_files.push_back(file);

                // Add to display list (filename only for brevity)
                m_filesFound.append(QString::fromStdString(entry.path().filename().string()));
            }
        }
    } catch (const fs::filesystem_error &e) {
        setStatus(QString("Error: %1").arg(e.what()));
    }
}

void OrganizerController::scan(sortType type, const bool includeHidden, const bool includeSystem, const DateGranularity granularity)
{
    if (m_directoryPath.isEmpty()) {
        setStatus("No directory selected");
        return;
    }

    fs::path dirPath = m_directoryPath.toStdString();

    if (!fs::exists(dirPath)) {
        setStatus("Directory does not exist");
        return;
    }

    if (!fs::is_directory(dirPath)) {
        setStatus("Path is not a directory");
        return;
    }

    setStatus("Scanning...");

    // Collect files
    collectFiles(dirPath);
    emit filesFoundChanged();
    emit fileCountChanged();

    // Plan operations
    FileOrganizer organizer(m_files);
    OrganizeOptions options;
    options.includeHidden = includeHidden;
    options.includeSystem = includeSystem;
    options.granularity = granularity;

    m_operations = organizer.planOperations(dirPath, type, options);

    // Update planned operations display
    m_plannedOperations.clear();
    for (const auto &op : m_operations) {
        QString display = QString::fromStdString(op.sourcePath.filename().string())
                        + " → "
                        + QString::fromStdString(op.destinationPath.parent_path().filename().string())
                        + "/";
        m_plannedOperations.append(display);
    }

    emit plannedOperationsChanged();
    emit operationCountChanged();
    emit canExecuteChanged();

    setStatus(QString("Found %1 files, %2 operations planned")
              .arg(m_files.size())
              .arg(m_operations.size()));
}

void OrganizerController::execute()
{
    if (m_operations.empty()) {
        setStatus("No operations to execute");
        return;
    }

    setStatus("Executing...");

    bool success = m_executor.executeAllOperations(m_operations);
    m_hasExecuted = true;

    emit canUndoChanged();

    if (success) {
        setStatus(QString("Successfully moved %1 files").arg(m_operations.size()));
    } else {
        setStatus("Some operations failed - check console for details");
    }
}

void OrganizerController::undoAll()
{
    if (!m_hasExecuted) {
        setStatus("Nothing to undo");
        return;
    }

    setStatus("Undoing...");

    bool success = m_executor.undoAll();

    if (success) {
        m_hasExecuted = false;
        emit canUndoChanged();
        setStatus("All operations undone");
    } else {
        setStatus("Some undo operations failed");
    }
}