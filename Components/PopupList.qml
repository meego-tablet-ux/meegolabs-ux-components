/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: outer

    property variant popupListModel
    property real itemHeight:30
    property variant value
    property int count: popupListModel.count
    property variant selectedValue

    property bool initComplete: false

    signal valueSelected (int index, variant tag)

    function reInit()
    {
        console.log("value obtained="+outer.value+"model count="+view.model.count);
        var focusIndex = 0;
        for (var i=0; i<view.model.count; i++) {
            if (view.model.get(i).tag == outer.value) {
                console.log("view.model.get(i).tag="+view.model.get(i).tag+", outer.value="+outer.value);
                focusIndex = i;
            }
        }

        console.log("********Inside reInit, view.currentIndex="+view.currentIndex+", focusIndex ="+focusIndex);
        if(view.currentIndex != focusIndex)
            view.currentIndex = focusIndex;

    }

    // find the current index matching the value of the button.
    function getCurrentIndex() {
        var i
        for (i=0; i<view.model.count; i++) {
            if (view.model.get(i).tag == value) {
                console.log("getCurrentIndex() view.model.get(i).tag="+view.model.get(i).tag+", value="+value);
                return i
            }
        }
        return 0
    }

    function setValues(index,currIndex,tag)
    {
        if(index == currIndex) {
            console.log("setValues() Setting values and returning true,index="+index+", currIndex="+currIndex+", tag="+tag);
            selectedValue=index;
            outer.valueSelected(index,tag);
            return true;
        }
        return false;
    }

    Component {
        id: tsdelegate

        Text {
           id: delegateText
           font.pixelSize: theme_fontPixelSizeSmall
           height: font.pixelSize + 10
           text: (tag == undefined) ? index : tag
           color: "#A0A0A0"
           font.bold: index == view.currentIndex
           property bool valueSet: setValues(index,view.currentIndex,tag)

           states: [
               State {
                   name: "active"
                   when:  index == view.currentIndex
                   PropertyChanges { target: delegateText; font.bold: true; color: theme_fontColorNormal; font.pixelSize: theme_fontPixelSizeLargest3;height:60}
               }
           ]
        }
    }


    Item {
        id: spinnerRect
        focus: true
        clip: true
        anchors.fill: parent
        BorderImage {
            id: spinner
            source: "image://theme/pickers/timespinbg"
            opacity: 0.5
            anchors.fill: parent
        }

        PathView {
            id: view
            anchors.fill: spinnerRect
            model: popupListModel
            BorderImage {
                id: innerBgImage
                source:"image://theme/pickers/timespinhi"
                anchors.fill: parent
                opacity:0.25
            }

            pathItemCount: 3
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            dragMargin: view.width/2
            delegate: tsdelegate

            path: Path {
                startX: view.width/2; startY: 5
                PathLine {
                    x: view.width/2;
                    y: view.pathItemCount * itemHeight + itemHeight
                }
            }

            Component.onCompleted: {
                console.log("value obtained="+outer.value);
                var focusIndex = 0;
                for (var i=0; i<view.model.count; i++) {
                    if (view.model.get(i).tag == outer.value) {
                        console.log("view.model.get(i).tag="+view.model.get(i).tag+", outer.value="+outer.value);
                        focusIndex = i;
                    }
                }

                console.log("Inside oncompleted, focusIndex ="+focusIndex);                
                view.currentIndex = focusIndex;
            }
        }

    }//end spinnerRect

}//end Item

