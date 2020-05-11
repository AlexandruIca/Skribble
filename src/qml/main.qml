import QtQuick.Controls 2.14
import QtQuick 2.2
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.2

import WorkArea 1.0

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
                icon.source: "icons/undo.png"

                action: Action {
                    shortcut: "Ctrl+z"
                    onTriggered: console.log("Action 'Undo'");
                }
            }
            MenuItem {
                text: qsTr("Redo")
                icon.source: "icons/redo.png"

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

        WorkArea {}
    }

    Rectangle {
        id: toolbar
        color: "#f5f5f5"
        width: parent.width / 15
        height: parent.height
        anchors.right: parent.right


        GridLayout {
            id: gridColors
            anchors.top: parent.top
            columns: 2
            columnSpacing: 10
            rowSpacing: 10
            Rectangle {
                Layout.topMargin: 10
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#000000"
            }
            Rectangle {
                Layout.topMargin: 10
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ffffff"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#696969"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#dcdcdc"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ff0000"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#a20005"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ff9ecb"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ff7d00"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ffff00"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ffc200"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#008000"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#76e523"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#0d33ff"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#61daff"
            }
            Rectangle {
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#800080"
            }
            Rectangle {
                Layout.rightMargin: 10
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#c597e8"
            }
            Rectangle {
                Layout.leftMargin: 10
                id: customColor1
                border.color: "black"
                radius: 8
                width: 40
                height: 40
                color: "#ffffff"
            }
            Rectangle {
                Layout.rightMargin: 10
                id: colorPicker
                border.color: "black"
                radius: 8
                width: 40
                height: 40

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "red" }
                    GradientStop { position: 0.2; color: "yellow" }
                    GradientStop { position: 0.4; color: "green" }
                    GradientStop { position: 0.6; color: "blue" }
                    GradientStop { position: 0.8; color: "magenta" }
                    GradientStop { position: 1.0; color: "red" }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.open()
                    }
                }
            }

        }

    }

    ColorDialog {
        id: colorDialog
        title: "Choose a color"
        onAccepted: {
            console.log("You chose: " + colorDialog.color)
            customColor1.color = colorDialog.color
            colorDialog.close()
        }
        onRejected: {
            console.log("Canceled")
            colorDialog.close()
        }
    }


}
