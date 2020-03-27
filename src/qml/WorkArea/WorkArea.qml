import QtQuick 2.12

Rectangle {
    width: 400
    height: 600
    color: "white"
    anchors.centerIn: parent

    MouseArea {
        id: dragArea
        anchors.fill: parent
        onWheel: {
            if(wheel.modifiers & Qt.ControlModifier) {
                parent.rotation += wheel.angleDelta.y / 120 * 5;
                if(Math.abs(parent.rotation) < 4) {
                    parent.rotation = 0;
                }
            }
            else {
                parent.rotation += wheel.angleDelta.x / 120;
                if(Math.abs(parent.rotation) < 0.6) {
                    parent.rotation = 0;
                }
                parent.scale += parent.scale * wheel.angleDelta.y / 120 / 10;
            }
        }
        onPositionChanged: {
            var pnt = Qt.point(Math.floor(mouseX), Math.floor(mouseY));
            if(parent.contains(pnt)) {
                console.log(pnt)
            }
        }
    }
}
