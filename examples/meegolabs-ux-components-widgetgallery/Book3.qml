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

PageDummy {

    id: pageDummy

    innerText: "book 3, page 1"
    rectColor: "yellow"
    showButton: true
    buttonLabel: "Page 2"
    pageTitle: "Dummy book 3"

    onClicked: {
        addPage( page2 )
    }

    Component{
        id: page2;

        PageDummy {
            id: pageDummy2

            innerText: "book 3, page 2"
            rectColor: "darkblue"
            showButton: true
            buttonLabel: "Page 3"
            pageTitle: "book 3, page 2"

            onClicked: { addPage( page3 )
            }
        }
    }

    Component{
        id: page3;

        PageDummy {
            id: pageDummy3

            innerText: "book 3, page 3"
            rectColor: "lightgreen"
            pageTitle: "book 3, page 3"
            buttonLabel: "Page 4"
            showButton: true

            onClicked: { addPage( page4 ) }
        }
    }

    Component{
        id: page4;

        PageDummy {
            id: pageDummy4

            innerText: "book 3, page 4"
            rectColor: "slategrey"
            pageTitle: "book 3, page 4"
            buttonLabel: "Page 5"
            showButton: true
            fullContent: false

            onClicked: { addPage( page5 ) }
        }
    }

    Component{
        id: page5;

        PageDummy {
            id: pageDummy5

            innerText: "book 3, page 5"
            rectColor: "slategrey"
            pageTitle: "book 3, page 5"
            showButton: true
            buttonLabel: "Page 6"
            fullScreen: false
            fullContent: true

            onClicked: { addPage( page6 ) }
        }
    }

    Component{
        id: page6;

        PageDummy {
            id: pageDummy6

            innerText: "book 3, page 6"
            rectColor: "slategrey"
            pageTitle: "book 3, page 6"
            showButton: true
            buttonLabel: "back"
            fullScreen: true
            fullContent: true

            onClicked: { popPage( page6 ) }
        }
    }


}


