import QtQuick 2.14
import QtQuick.Controls 2.14

import Backend.Canvas 1.0

Rectangle {
    anchors.centerIn: parent
    width: 400
    height: 600
    focus: true

    SkCanvas {
        id: canvas
        anchors.fill: parent
    }

    transform: [
        Scale {
            id: tform
            origin.x: parent.x
            origin.y: parent.y
        },
        Translate {
            id: translate
            x: 0
            y: 0
        }
    ]

    Keys.onPressed: {
        const offset = 10;
        if(event.key === Qt.Key_Left) {
            translate.x += offset;
            event.accepted = true;
        }
        else if(event.key === Qt.Key_Right) {
            translate.x -= offset;
            event.accepted = true;
        }
        else if(event.key === Qt.Key_Up) {
            translate.y += offset;
            event.accepted = true;
        }
        else if(event.key === Qt.Key_Down) {
            translate.y -= offset;
            event.accepted = true;
        }
        else if(event.key === Qt.Key_C) {
            translate.y = 0;
            translate.x = 0;
            rotation = 0;
            x = parent.width / 2 - x / 2;
            y = parent.height / 2 - y / 2;
            tform.xScale = 1;
            tform.yScale = 1;
            event.accepted = true;
        }
        else if(event.key === Qt.Key_U) {
            canvas.undo();
            event.accepted = true;
        }
        else if(event.key === Qt.Key_R) {
            canvas.redo();
            event.accepted = true;
        }
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
                var zoomFactor;
                if(wheel.angleDelta.y > 0) {
                    zoomFactor = factor;
                }
                else {
                    zoomFactor = 1 / factor;
                }

                const origin = mapToItem(parent.parent, wheel.x, wheel.y);
                tform.origin.x = origin.x - translate.x;
                tform.origin.y = origin.y - translate.y;

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
                canvas.mousePositionChanged(pnt);
            }
        }
        onReleased: {
            canvas.mouseReleased();
        }
    }
}
