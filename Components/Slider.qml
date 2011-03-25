/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: container
    width: 200
    height: 40

    property int min: 0
    property int max: 100

    property int value: 0//(max - min) * (percent / 100) + min

    property int percent: 0//((100*(min - value))/(max-min)) * -1
    property bool callbackflag: false

    property bool textOverlayVertical: false

    function moveSlider(val)
    {
        callbackflag = true;
        value = val
        console.log("updating percentage")
       /* if(!callbackflag){
            marker.x = container.width * (val / 100);
            //    console.log("moving marker to: " + marker.x + " width: " + container.width + " value: " + val)
        }
        else callbackflag = false;

*/
    }

    signal sliderChanged(int sliderValue)

    Rectangle {
        id: fillArea
        radius: 10
        anchors.fill: parent
        anchors.margins: 10

        color: "black"

        Rectangle {
            //id: sliderFill
            radius: 20
            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            anchors.left: parent.left
            width: marker.x
            color: "lightblue"
            opacity: 0.5
        }

        function mapFromPosToPercent(mouseX) {
            var pos = fillArea.width - mouseX;
           // pos = pos < marker.width? marker.width: pos;
           // pos = pos > fillArea.width ? fillArea.width: pos;

            // invert X coordinate to screen coordinate
            pos = fillArea.width - pos;

            //var value =  (fillArea.width - marker.width - pos)/(fillArea.width);
            var value =  (pos)/(fillArea.width);
            if (value < 0) {
                value = 0;
            }
            if (value > 1) {
                value = 1;
            }
            return Math.floor(value*100);
        }
    }

    Item {
        id: textoverlay
        anchors.bottom: marker.top
        anchors.bottomMargin:5
        anchors.horizontalCenter: marker.horizontalCenter
        width: overlaytext.width * 1.25
        height: overlaytext.height * 1.25
        visible: false

        rotation: container.textOverlayVertical? 90 : 0


        Rectangle {
            id: overlaybackground
            radius: 5
            anchors.fill: parent
            color: "lightblue"
            opacity: 0.5
        }

        Text {
            id: overlaytext
            anchors.centerIn: overlaybackground
            text: value
        }
    }

    Image {
        id: marker
        anchors.verticalCenter: parent.verticalCenter
        source: "image://theme/scrub_head_lrg"
        x: (percent/100) * (fillArea.width - (marker.width /2 ))
       //scale: 0.5
    }

    MouseArea {
        id: mouseAreaFTW
        anchors.fill: fillArea
        property bool mousePressed: false

        onPressed: {
           // percent = (fillArea.mapFromPosToPercent(mouseX));
            value = (max - min) * (fillArea.mapFromPosToPercent(mouseX) / 100) + min
            sliderChanged(percent)
            console.log("pressed: percentage:" + percent)
        }
        onPositionChanged: {
            //percent = (fillArea.mapFromPosToPercent(mouseX));
            value = (max - min) * (fillArea.mapFromPosToPercent(mouseX) / 100) + min
            sliderChanged(percent)
            console.log("dragged: percentage:" + percent)
        }
    }

        /*onPressed: {
            mousePressed = true;
            var tempx
            tempx = mouseX - marker.width / 2;
            container.percent = ((tempx + marker.width / 2) / fillArea.width) * 100
            container.value =  (max - min) * (percent / 100) + min
            marker.x = tempx
            callbackflag = true;
        }

        onPositionChanged: {
            if(mousePressed == true) {
                var tempx = 0;
                if(mouseX < fillArea.width - marker.width && mouseX > 0)
                    tempx = mouseX - marker.width / 2;
                else tempx = fillArea.width

                if(mouseX < 0)
                    tempx = fillArea.x
                else if(mouseX < fillArea.width)
                    tempx = mouseX - marker.width / 2

                container.percent = ((tempx + marker.width / 2) / fillArea.width) * 100
                container.value = (max - min) * (percent / 100) + min
                marker.x = tempx
                callbackflag = true;
            }
        }

        onReleased: {
            mousePressed = false;

        }*/

    ///"I hate the way this works" -tripzero

    onValueChanged: {
       callbackflag = true
       percent = ((100*(min - value))/(max-min)) * -1
    }

    onPercentChanged: {
        if(!callbackflag) {
            console.log("firing signal")
            sliderChanged(percent)
        }
        else console.log("not firing signal")
        callbackflag = false;
    }

    states: [
        State {
            name: "marker"
            PropertyChanges {
                target: textoverlay
                visible: true
            }
            when: mouseAreaFTW.pressed
        }
    ]
    transitions: [
        Transition {
            NumberAnimation { target: marker; property: "x"; duration: 1000 }
        }
    ]

}
