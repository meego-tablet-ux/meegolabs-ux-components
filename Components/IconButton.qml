/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import Qt.labs.gestures 2.0

Item {
    id: container

    property string icon
    property string iconDown
    property alias fillMode: image.fillMode
    property bool pressed: false
    property bool imageFill: false
    property bool active: true
    width: image.width
    height: image.height

    opacity: active ? 1.0 : 0.5

    signal clicked(variant mouse)

    Image {
        id: image
        anchors.centerIn: imageFill ? undefined : parent
        anchors.fill: imageFill ? parent : undefined
        width: sourceSize.width
        height: sourceSize.height
        source: icon
        fillMode: Image.PreserveAspectCrop
        states: [
            State {
                name: "pressed"
                when: container.pressed
                PropertyChanges {
                    target: image
                    source: iconDown == "" ? icon : iconDown
                }
            }
        ]
    }

    GestureArea {
        anchors.fill: parent
        Tap {
            onFinished: {
                if (container.active)
                    container.clicked(gesture.position)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if (container.active) {
                parent.pressed = true;
            }
        }
        onReleased: {
            if (container.active) {
                parent.pressed = false;
            }
        }
    }
}
