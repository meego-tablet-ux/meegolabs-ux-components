/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

AbstractContext {
    id: outer

    signal closed
    signal opened
    signal triggered(variant date)
    signal cancel
    property int xVal
    property int yVal
    property int innerOpacity:0
    property int yearVal:-1
    property int monthVal:-1
    property int dayVal:-1


    function show(x,y) {
        outer.mouseX = x
        outer.mouseY = y
        console.log("mouseX="+outer.mouseX+", mouseY="+outer.mouseY);
        outer.visible = true;
        outer.innerOpacity=1;
        outer.fogOpacity=0
        outer.modalSurfaceItem.closeUponFogClicked = false;
        outer.opened();
    }

    function forceClose() {
        if (outer.innerOpacity != 0) {
            outer.innerOpacity = 0;
            outer.visible = false;
            outer.closed();
       }
    }


    // Contents: Date Picker goes here...
    content: Item {
        id: inner

        z: 2;
        opacity: innerOpacity
        visible: innerOpacity > 0
        width: 360; height: 550

        Behavior on opacity { NumberAnimation { duration: 320 } }

        DatePicker {
            id: datePicker

            year: yearVal
            month: monthVal
            day: dayVal

            //Returns the selected date
            onDateSelected: {
                if (date == undefined) {
                    outer.cancel()
                } else {
                    outer.triggered(date)
                }

                forceClose()
            }
        }
    }


    states: [
        State {
            name: "inactive"
            when: innerOpacity == 0
            PropertyChanges { target: outer; visible: false }
        },
        State {
            name: "active"
            when:  innerOpacity > 0
            PropertyChanges { target: outer; visible: true }
        }
    ]

}
