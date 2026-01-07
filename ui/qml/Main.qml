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

    color: "#1e1e1e"  // Dark theme

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
                text: ""
                color: "#ffffff"
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                    border.color: "#3d3d3d"
                }
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
                model: ["Category", "Extension"]
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

            Item { Layout.fillWidth: true }  // Spacer
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
                        text: "Files Found (0)"
                        font.bold: true
                        color: "#ffffff"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: []  // Will be populated by controller

                        delegate: Label {
                            text: modelData
                            color: "#cccccc"
                            padding: 4
                        }
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
                        text: "Planned Operations"
                        font.bold: true
                        color: "#ffffff"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: []  // Will be populated by controller

                        delegate: Label {
                            text: modelData
                            color: "#cccccc"
                            padding: 4
                        }
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
                    console.log("Scanning: " + directoryField.text)
                    // TODO: Call controller.scan()
                }
            }

            Button {
                text: "Execute"
                enabled: false  // Enable when operations are planned
                onClicked: {
                    console.log("Executing operations")
                    // TODO: Call controller.execute()
                }
            }

            Button {
                text: "Undo All"
                enabled: false  // Enable when operations are completed
                onClicked: {
                    console.log("Undoing all operations")
                    // TODO: Call controller.undoAll()
                }
            }

            Item { Layout.fillWidth: true }  // Spacer

            Label {
                id: statusLabel
                text: "Status: Ready"
                color: "#888888"
            }
        }
    }

    // Folder picker dialog
    FolderDialog {
        id: folderDialog
        title: "Select Directory to Organize"
        onAccepted: {
            directoryField.text = selectedFolder.toString().replace("file://", "")
        }
    }
}