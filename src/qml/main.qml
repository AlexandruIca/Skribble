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
        id: wrect
        anchors.fill: parent
        color: "#6b6b6b"

        WorkArea {
            id: warea
        }
    }

    Rectangle {
        id: colorPickerBar
        color: "#f5f5f5"
        width: parent.width / 10
        height: parent.height
        anchors.right: parent.right

        GridLayout {
            id: gridColors
            anchors.top: parent.top
            columns: 4
            columnSpacing: 10
            rowSpacing: 10
            Rectangle {
                id: color1
                Layout.topMargin: 10
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#000000"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color1.color)
                        currentColor.color = color1.color
                    }
                }
            }
            Rectangle {
                id: color2
                Layout.topMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#696969"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color2.color)
                        currentColor.color = color2.color
                    }
                }
            }
            Rectangle {
                id: color3
                Layout.topMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#dcdcdc"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color3.color)
                        currentColor.color = color3.color
                    }
                }
            }
            Rectangle {
                id: color4
                Layout.topMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffffff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color4.color)
                        currentColor.color = color4.color
                    }
                }
            }
            Rectangle {
                id: color5
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#a20005"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color5.color)
                        currentColor.color = color5.color
                    }
                }
            }
            Rectangle {
                id: color6
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ff0000"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color6.color)
                        currentColor.color = color6.color
                    }
                }
            }
            Rectangle {
                id: color7
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ff9ecb"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color7.color)
                        currentColor.color = color7.color
                    }
                }
            }
            Rectangle {
                id: color8
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffff00"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color8.color)
                        currentColor.color = color8.color
                    }
                }
            }
            Rectangle {
                id: color9
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ff7d00"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color9.color)
                        currentColor.color = color9.color
                    }
                }
            }
            Rectangle {
                id: color10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffc200"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color10.color)
                        currentColor.color = color10.color
                    }
                }
            }
            Rectangle {
                id: color11
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#76e523"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color11.color)
                        currentColor.color = color11.color
                    }
                }
            }
            Rectangle {
                id: color12
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#008000"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color12.color)
                        currentColor.color = color12.color
                    }
                }
            }
            Rectangle {
                id: color13
                Layout.leftMargin: 10
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#0d33ff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color13.color)
                        currentColor.color = color13.color
                    }
                }
            }
            Rectangle {
                id: color14
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#61daff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color14.color)
                        currentColor.color = color14.color
                    }
                }
            }
            Rectangle {
                id: color15
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#c597e8"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color15.color)
                        currentColor.color = color15.color
                    }
                }
            }
            Rectangle {
                id: color16
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#800080"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(color16.color)
                        currentColor.color = color16.color
                    }
                }
            }
            Rectangle {
                Layout.leftMargin: 10
                id: customColor1
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffffff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(customColor1.color)
                        currentColor.color = customColor1.color
                    }
                }
            }
            Rectangle {
                id: customColor2
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffffff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(customColor2.color)
                        currentColor.color = customColor2.color
                    }
                }
            }
            Rectangle {
                id: customColor3
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffffff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(customColor3.color)
                        currentColor.color = customColor3.color
                    }
                }
            }
            Rectangle {
                id: customColor4
                border.color: "black"
                radius: 8
                width: 20
                height: 20
                color: "#ffffff"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        warea.callChangeColor(customColor4.color)
                        currentColor.color = customColor4.color
                    }
                }
            }
            Rectangle {
                Layout.leftMargin: 10
                id: colorPicker
                border.color: "black"
                radius: 8
                width: 20
                height: 20

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
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        colorDialog.open()
                    }
                }
            }

        }

        Label {
            id: customLabel
            text: "Current color:"
            font.pixelSize: 15/100 * parent.width
            color: "black"
            anchors.top: gridColors.bottom
            anchors.topMargin: 10
            anchors.left: colorPickerBar.left
            anchors.leftMargin: 10
        }

        Rectangle {
            id: currentColor
            width: 40
            height: 40
            color: "black"
            border.color: "black"
            radius: 8
            anchors.top: customLabel.bottom
            anchors.topMargin: 10
            anchors.left: colorPickerBar.left
            anchors.leftMargin: 10

        }

    }

    ColorDialog {
        id: colorDialog
        title: "Choose a color"
        property int nr : 1;
        onAccepted: {
            console.log("You chose: " + colorDialog.color);

            if (nr === 1) {
                customColor1.color = colorDialog.color
                nr = 2
            }
            else if (nr === 2) {
                customColor2.color = colorDialog.color
                nr = 3
            }
            else if (nr === 3) {
                customColor3.color = colorDialog.color
                nr = 4
            }
            else if (nr === 4) {
                customColor4.color = colorDialog.color
                nr = 1
            }

            warea.callChangeColor(colorDialog.color)
            currentColor.color = colorDialog.color
            colorDialog.close()
        }
        onRejected: {
            console.log("Canceled")
            colorDialog.close()
        }
    }


}
