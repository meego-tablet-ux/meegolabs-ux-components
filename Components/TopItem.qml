/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
    id: top

    //This should be read only, but not possible currently in QML...
    //See http://bugreports.qt.nokia.com/browse/QTBUG-15257
    property Item topItem: null

    Component.onCompleted: calcTopParent()

    Connections {
        target: topItem
        onParentChanged: calcTopParent()
    }

    function calcTopParent() {
        var p = top;
        var lastp;
        while(p) {
            if(p.toString().indexOf("Window_QMLTYPE") == 0) {
                topItem = p.content;
                break;
            }
            lastp = p;
            p = p.parent;
        }
        //Try to fallback to the last valid item, if we don't find a window type...
        if (!p)
            topItem = lastp;
    }
}
