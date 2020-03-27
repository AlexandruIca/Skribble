import QtQuick 2.12

Rectangle {
    width: 400
    height: 600
    color: "white"
    anchors.centerIn: parent

    transform: Scale {
        id: tform
        origin.x: parent.x
        origin.y: parent.y
    }

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
                /*
                parent.rotation += wheel.angleDelta.x / 120;
                if(Math.abs(parent.rotation) < 0.6) {
                    parent.rotation = 0;
                }
                parent.scale += parent.scale * wheel.angleDelta.y / 120 / 10;*/

                const factor = 1.05;

                if(wheel.angleDelta.y > 0) {
                    var zoomFactor = factor;
                }
                else {
                    var zoomFactor = 1 / factor;
                }

                const origin = mapToItem(parent.parent, wheel.x, wheel.y);
                tform.origin.x = origin.x;
                tform.origin.y = origin.y;

                const realX = wheel.x * tform.xScale;
                const realY = wheel.y * tform.yScale;
                parent.x += (1 - zoomFactor) * realX;
                parent.y += (1 - zoomFactor) * realY;
                tform.xScale *= zoomFactor;
                tform.yScale *= zoomFactor;
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
