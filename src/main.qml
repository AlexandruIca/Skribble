import QtQuick 2.12
import QtQuick.Controls 2.14

ApplicationWindow {
    visible: true
    id: page
    width: 1280
    height: 720
    title: qsTr("Skribble")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Proiect")

            MenuItem {
                text: qsTr("&New...")
                onTriggered: console.log("Action 'New'");
            }
            MenuItem {
                text: qsTr("Open...")

                action: Action {
                    shortcut: "Ctrl+o"
                    onTriggered: console.log("Action 'Open'");
                }
            }
            MenuItem {
                text: qsTr("Save")

                action: Action {
                    shortcut: "Ctrl+s"
                    onTriggered: console.log("Action 'Save'");
                }
            }
            MenuItem {
                text: qsTr("Exit")

                action: Action {
                    shortcut: "Ctrl+q"
                    onTriggered: Qt.quit();
                }
            }
        }

        Menu {
            title: qsTr("Edit")

            MenuItem {
                text: qsTr("Undo")

                action: Action {
                    shortcut: "Ctrl+z"
                    onTriggered: console.log("Action 'Undo'");
                }
            }
            MenuItem {
                text: qsTr("Redo")

                action: Action {
                    shortcut: "Ctrl+r"
                    onTriggered: console.log("Action 'Redo'");
                }
            }
        }

        Menu {
            title: qsTr("Help")

            MenuItem {
                text: qsTr("About")
                onTriggered: Qt.openUrlExternally("https://github.com/AlexandruIca/Skribble");
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#6b6b6b"
    }
}
