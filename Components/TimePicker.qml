/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Settings 0.1

/*
 * Time Picker
 *
 * returns selected hours and minutes
 *
 * parameters:
 *   hours - currently showing hours (1-12 or 1-24)
 *   minutes - currently value for minutes. not padded string
 *   minutesIncrement - number of minutes to increment minutes by: must go into 60 evenly
 *   ampm - string set to either "AM" or "PM". not shown if hr24 = false
 *   time - string representation of the time shown
 *   hr24 - true if time is in 24 hour format
 *   promptString - string displayed at the top
 *
 * functions:
 *  show(xVal,yVal) - shows the time picker with the current time displayed
 */

AbstractContext {
    id: timePicker

    property int hours
    property int minutes
    property bool hr24:time24.flag24
    property int minutesIncrement: 1
    property string promptString: qsTr("Pick a time")
    property string ampmToggleValue

    property string minutesPadded: (minutes < 10 ? "0" : "") + minutes
    property string ampm: hr24 ? "" : ampmToggleValue
    property string time: hours + ":" + minutesPadded + " " + ampm

    property int innerOpacity:0
    signal closed
    signal opened
    signal cancelled

    function show(xVal,yVal) {
        timePicker.mouseX = xVal
        timePicker.mouseY = yVal
        timePicker.visible = true
        timePicker.innerOpacity = 1;
        timePicker.visible = true;
        timePicker.fogOpacity=0
        timePicker.modalSurfaceItem.closeUponFogClicked = false;
        timePicker.opened();
    }

    function setValues() {
        minutesPadded=(minutes < 10 ? "0" : "") + minutes
        ampm=(hr24) ? "" : ampmToggleValue
        time=hours + ":" + minutesPadded + " " + ampm
    }

    function forceClose(buttonVal) {
        if(buttonVal==1)
            timePicker.closed();
        else
            timePicker.cancelled();
        if (timePicker.innerOpacity != 0) {
            timePicker.innerOpacity = 0;
            timePicker.visible = false;
       }
    }


    TimeSettings {
        id:time24
    }

    content: Item {
        id: inner
        opacity: innerOpacity
        visible: innerOpacity > 0
        width: 300; height: (titleBox.height)+spinnerBox.height+(( hr24 ? 0 : ampmToggleBox.height ))+buttonBox.height+25
        Behavior on opacity { NumberAnimation { duration: 250 } }

        Item {
            id: backgroundRect

            property int hours: 12
            property int minutes: 0

            anchors.fill: parent
            clip:  true

            Column {
                id:pickerContents
                spacing: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.left: parent.left

                Item {
                    id:titleBox
                    height:30
                    width:backgroundRect.width
                    Text {
                        id: promptText
                        text: timePicker.promptString
                        color:theme_fontColorNormal
                        font.pixelSize: theme_fontPixelSizeLarge
                        anchors.centerIn: parent
                    } // prompt text
                }//titleBox

                Item {
                    id:spinnerBox
                    height:130
                    width:backgroundRect.width

                    Row {
                        id:spinnersRow
                        spacing: 5
                        anchors.margins: 10
                        anchors.centerIn: parent
                        Item {
                            id:innerBox
                            height:spinnerBox.height
                            width:spinnerBox.width-20
                            TimeSpinner {
                                id: hourSpinner
                                min: (hr24 ? 0:1)
                                incr: 1
                                count: (hr24 ? 24 : 12)
                                pad: false
                                anchors { left: parent.left; right: parent.horizontalCenter; top: parent.top; bottom: parent.bottom;
                                    leftMargin: 14; rightMargin: 14; topMargin: 10; bottomMargin:10;
                                }

                            } // hours

                            Item {
                                id:colonBox
                                height:spinnerBox.height
                                anchors.left: hourSpinner.right
                                anchors.right: minutesSpinner.left
                                Text {
                                    id: colon
                                    text: ":"
                                    anchors.centerIn:parent
                                    color:theme_fontColorNormal
                                    font.pixelSize: theme_fontPixelSizeLargest3
                                }//colon
                            }


                            TimeSpinner {
                                id: minutesSpinner
                                min: 0
                                incr: minutesIncrement
                                count: 60 / incr
                                pad: false
                                anchors { left: parent.horizontalCenter; right: parent.right; top: parent.top;bottom: hourSpinner.bottom;
                                   leftMargin: 14; rightMargin: 14; topMargin: 10;
                                }
                            } // minutes
                        }


                    }//end row

                }//end spinnerBox

                Item {
                    id:ampmToggleBox
                    width:backgroundRect.width
                    height:(!hr24)?40:0
                    ToggleButton {
                        id: ampmToggle
                        z:50
                        visible: !hr24
                        onLabel: qsTr("AM")
                        offLabel: qsTr("PM")
                        anchors.centerIn: parent
                        onToggled: {
                            ampmToggleValue = ampmToggle.on ? ampmToggle.onLabel : ampmToggle.offLabel;
                        }

                    }//togglebutton
                } //ampmToggleBox

                Item {
                    id:buttonBox
                    width:parent.width
                    height: 50
                    Button {
                        id:okButton
                        width:buttonBox.width/(2.5)
                        height:40
                        bgSourceUp: "image://theme/btn_blue_up"
                        bgSourceDn: "image://theme/btn_blue_dn"
                        title: qsTr("OK")
                        font.pixelSize: theme_fontPixelSizeLarge
                        color: theme_buttonFontColor
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            hours = hourSpinner.value;
                            hours=hr24 ? hours:((ampmToggle.on) ? ((hours==12)?0:hours):((hours<12)?(hours+12):hours))
                            minutes = minutesSpinner.value
                            timePicker.setValues();
                            timePicker.forceClose(1);
                        }

                    }

                    Button {
                        id:cancelButton
                        width:buttonBox.width/(2.5)
                        height:40
                        bgSourceUp: "image://theme/btn_grey_up"
                        bgSourceDn: "image://theme/btn_grey_dn"
                        title: qsTr("Cancel")
                        font.pixelSize: theme_fontPixelSizeLarge
                        color: theme_buttonFontColor
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            timePicker.forceClose(2);
                        }
                    }

                }//buttonBox

            }


        } // backgroundRect

    } // inner

}
