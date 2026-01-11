#ifndef ORGANIZERCONTROLLER_H
#define ORGANIZERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>

#include "FileEntry.h"
#include "FileOrganizer.h"
#include "OperationExecutor.h"

class OrganizerController : public QObject
{
    Q_OBJECT

    // Properties exposed to QML
    Q_PROPERTY(QString directoryPath READ directoryPath WRITE setDirectoryPath NOTIFY directoryPathChanged)
    Q_PROPERTY(QStringList filesFound READ filesFound NOTIFY filesFoundChanged)
    Q_PROPERTY(QStringList plannedOperations READ plannedOperations NOTIFY plannedOperationsChanged)
    Q_PROPERTY(int fileCount READ fileCount NOTIFY fileCountChanged)
    Q_PROPERTY(int operationCount READ operationCount NOTIFY operationCountChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool canExecute READ canExecute NOTIFY canExecuteChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)

public:
    explicit OrganizerController(QObject *parent = nullptr);

    // Property getters
    QString directoryPath() const { return m_directoryPath; }
    QStringList filesFound() const { return m_filesFound; }
    QStringList plannedOperations() const { return m_plannedOperations; }
    int fileCount() const { return m_files.size(); }
    int operationCount() const { return m_operations.size(); }
    QString status() const { return m_status; }
    bool canExecute() const { return !m_operations.empty(); }
    bool canUndo() const { return m_hasExecuted; }

    // Property setters
    void setDirectoryPath(const QString &path);

    // Methods callable from QML
    Q_INVOKABLE void scan(sortType type, bool includeHidden, bool includeSystem, DateGranularity granularity);
    Q_INVOKABLE void execute();
    Q_INVOKABLE void undoAll();
    Q_INVOKABLE void setDirectoryFromUrl(const QUrl &url);

signals:
    void directoryPathChanged();
    void filesFoundChanged();
    void plannedOperationsChanged();
    void fileCountChanged();
    void operationCountChanged();
    void statusChanged();
    void canExecuteChanged();
    void canUndoChanged();

private:
    void setStatus(const QString &status);
    void collectFiles(const std::filesystem::path &path);

    QString m_directoryPath;
    QString m_status = "Ready";
    QStringList m_filesFound;
    QStringList m_plannedOperations;

    std::vector<FileEntry> m_files;
    std::vector<FileOperation> m_operations;
    OperationExecutor m_executor;
    bool m_hasExecuted = false;
};

#endif // ORGANIZERCONTROLLER_H