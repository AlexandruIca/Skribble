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
                const absRotation = Math.abs(Math.floor(parent.rotation)) % 360;
                const sign = parent.rotation < 0 ? -1 : 1;

                if(absRotation < 4) {
                    parent.rotation = 0;
                }
                else if(Math.abs(absRotation - 90) < 4) {
                    parent.rotation = 90 * sign;
                }
                else if(Math.abs(absRotation - 180) < 4) {
                    parent.rotation = 180 * sign;
                }
                else if(Math.abs(absRotation - 270) < 4) {
                    parent.rotation = 270 * sign;
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
            const pnt = Qt.point(Math.floor(mouseX), Math.floor(mouseY));
            if(parent.contains(pnt)) {
                console.log(pnt)
            }
        }
    }
}
