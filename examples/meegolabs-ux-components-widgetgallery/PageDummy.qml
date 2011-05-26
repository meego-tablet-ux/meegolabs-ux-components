/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This file is just meant as a dummy to quickly create pages for
   demonstrating the book/page concept. */
import Qt 4.7
import MeeGo.Components 0.1

AppPage {
    id: pageDummy

    property alias innerText: rectText.text //text shown in the rect
    property alias rectColor: innerRect.color   //color of the rect in the middle
    property alias showButton: nextButton.visible   //nextButton visible?
    property alias buttonLabel: nextButton.text    //nextButton label
    property string bookTitle: "book title" //shown in the title bar

    signal clicked()

    anchors.fill:  parent

    Rectangle { z: -1; anchors.fill: parent; color: "grey" } //background

    Rectangle {
        id: innerRect

        anchors.fill: parent
        anchors.margins:  50
        color: "lightgreen"

        Text {
            id: rectText

            text: "dummy page"
            anchors.centerIn: parent
            font.pixelSize: 40
            color: "black"
        }
    }

    Button {
        id: nextButton

        visible: false
        text: "next page >"
        anchors.right:  innerRect.right
        anchors.bottom:  innerRect.bottom
        anchors.margins: 10
        onClicked: { pageDummy.clicked() }
    }
}

