/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

/* This file contains relativy empty pages and is meant to demonstrate the
   book/page concept */

import Qt 4.7
import MeeGo.Components 0.1

PageDummy {
    id: pageDummy

    innerText: qsTr("book 2, page 1")
    rectColor: "lightblue"
    showButton: true
    buttonLabel: qsTr("Page 2")
    pageTitle: qsTr("Dummy book 2")

    onClicked: { addPage( page2 ) }

    onSearch: {}

    onSearchExtended: {}

    onSearchRetracted: {}

    Component{
        id: page2;

        PageDummy {
            id: pageDummy2

            enableCustomActionMenu: true

            onActionMenuIconClicked: {
                pageDummy.actionMenuOpen = true
                contextActionMenu.setPosition( mouseX, mouseY )
                contextActionMenu.show()
            }

            ContextMenu {
                id: contextActionMenu
                forceFingerMode: 2

                onVisibleChanged: {
                    actionMenuOpen = visible
                }

                title:  qsTr("Custom ContextMenu")

                content:  Rectangle {
                    width: 300
                    height: 300
                    color: "red"
                }
            }

            innerText: qsTr("book 2, page 2")
            rectColor: "lightgreen"
            showButton: true
            buttonLabel: qsTr("Page 3")
            pageTitle: qsTr("Dummy book 2")

            onClicked: { addPage( page3 )
            }
        }
    }

    Component{
        id: page3;

        PageDummy {
            id: pageDummy3

            enableCustomActionMenu: true

            onActionMenuIconClicked: {
                pageDummy.actionMenuOpen = true
                contextActionMenu.setPosition( mouseX, mouseY )
                contextActionMenu.show()
            }

            ContextMenu {
                id: contextActionMenu
                forceFingerMode: 2

                onVisibleChanged: {
                    actionMenuOpen = visible
                }

                title:  qsTr("Custom ContextMenu")

                content:  Rectangle {
                    width: 400
                    height: 200
                    color: "blue"
                }
            }

            innerText: qsTr("book 2, page 3")
            rectColor: "orange"
            showButton: true
            buttonLabel: qsTr("Page 4")
            pageTitle: qsTr("Dummy book 2")

            onClicked: { addPage( page4 )
            }
        }
    }

    Component{
        id: page4;

        PageDummy {
            id: pageDummy4

            innerText: qsTr("book 2, page 4")
            rectColor: "darkgrey"
            pageTitle: qsTr("Dummy book 2")
        }
    }
}

