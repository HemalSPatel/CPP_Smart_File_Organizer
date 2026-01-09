import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 650
    title: "Smart File Organizer"

    color: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Header - Directory Selection
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: directoryField
                Layout.fillWidth: true
                placeholderText: "Select a directory to organize..."
                text: controller.directoryPath
                color: "#ffffff"
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                    border.color: "#3d3d3d"
                }
                onTextChanged: controller.directoryPath = text
            }

            Button {
                text: "Browse"
                onClicked: folderDialog.open()
            }
        }

        // Options Row
        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            Label {
                text: "Organize by:"
                color: "#ffffff"
            }

            ComboBox {
                id: sortTypeCombo
                model: ["Category", "Extension", "Date"]
                currentIndex: 0
            }

            CheckBox {
                id: includeHiddenCheck
                text: "Include Hidden"
                checked: false
            }

            CheckBox {
                id: includeSystemCheck
                text: "Include System"
                checked: false
            }

            ComboBox {
                id: dateSortGranularity
                model: ["Year", "Month", "Day"]
                currentIndex: 0
            }

            Item { Layout.fillWidth: true }
        }

        // Main Content - Two Panels
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            // Left Panel - Files Found
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#2d2d2d"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12

                    Label {
                        text: "Files Found (" + controller.fileCount + ")"
                        font.bold: true
                        color: "#ffffff"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: controller.filesFound

                        delegate: Label {
                            text: modelData
                            color: "#cccccc"
                            padding: 4
                        }

                        ScrollBar.vertical: ScrollBar { }
                    }
                }
            }

            // Right Panel - Planned Operations
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#2d2d2d"
                radius: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12

                    Label {
                        text: "Planned Operations (" + controller.operationCount + ")"
                        font.bold: true
                        color: "#ffffff"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: controller.plannedOperations

                        delegate: Label {
                            text: modelData
                            color: "#cccccc"
                            padding: 4
                        }

                        ScrollBar.vertical: ScrollBar { }
                    }
                }
            }
        }

        // Action Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                text: "Scan"
                enabled: directoryField.text.length > 0
                onClicked: {
                    controller.scan(sortTypeCombo.currentIndex,
                        includeHiddenCheck.checked,
                        includeSystemCheck.checked)
                }
            }

            Button {
                text: "Execute"
                enabled: controller.canExecute
                onClicked: controller.execute()
            }

            Button {
                text: "Undo All"
                enabled: controller.canUndo
                onClicked: controller.undoAll()
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Status: " + controller.status
                color: "#888888"
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Directory to Organize"
        onAccepted: {
            controller.setDirectoryFromUrl(selectedFolder)
        }
    }
}