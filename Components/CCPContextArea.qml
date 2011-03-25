/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

MouseArea {
    property int clickCount: 0
    property int selectionStart: 0
    property int selectionEnd: 0

    property real xOffset: 0
    property real yOffset: 0
    property int pendingCursorPosition: 0

    // The Item this mouse area handles
    property Item editor: null

    // Set to true when we are actually currently dragging a selection
    // to prevent the context menu activating in the middle of the
    // drag
    property bool currentlySelecting: false

    // The mouse area needs to expand outside of the parent
    // so that the selection handles can be clicked when they
    // are in the gutter
    x: 0
    y: 0
    width: parent.width
    height: parent.height

    // Shows the context menu at cx,cy. cx & cy are in the root windows
    // coordinate space, not the CCPContextArea's.
    function showContextMenu (cx, cy) {
        clipboardContextMenu.displayContextMenu (cx, cy);
    }

    // We set the position here, which in turn controls where the
    // selectionHandleSurface places the handles.
    // It looks like the user is dragging the handles and that controls the
    // selection, but it is actually the opposite way round
    function setStartPosition (px, py) {
        var s = editor.positionAt (px, py);
        if (s > selectionEnd) {
            s = selectionEnd;
        }

        editor.select (s, selectionEnd);
        selectionStart = s;

        var rect = editor.positionToRectangle (selectionStart);
        var map = mapToItem (scene.content, rect.x, rect.y);
        selectionHandleSurface.startHandle.setPosition (map.x, map.y, rect.height);
    }

    function setEndPosition (px, py) {
        var e = editor.positionAt (px, py);
        if (e < selectionStart) {
            e = selectionStart;
        }

        editor.select (selectionStart, e);
        selectionEnd = e;

        var rect = editor.positionToRectangle (selectionEnd);
        var map = mapToItem (scene.content, rect.x, rect.y);
        selectionHandleSurface.endHandle.setPosition (map.x, map.y, rect.height);
    }

    Timer {
        id: doubleClickTimer
        interval: 250
        onTriggered: {
            parent.clickCount = 0;
            console.log ("Positioning\n");
            editor.cursorPosition = pendingCursorPosition;
            editor.forceActiveFocus ();
        }
    }

    function inHandle (h, mx, my) {
        return (mx >= h.x && mx <= h.x + h.width) && (my >= h.y && my <= h.y + h.height);
    }

    onPressed: {
        doubleClickTimer.stop ();
        clickCount++;
        // Start double click timer
        // If the timer expires before another press event
        // then the click count will be reset
        doubleClickTimer.start ();

        if (clickCount == 2) {
            state = "selection"

            selectionStart = editor.positionAt (mouse.x, mouse.y);
            selectionEnd = selectionStart;

            var rect = editor.positionToRectangle (selectionStart);
            var map = mapToItem (scene.content, rect.x, rect.y);
            selectionHandleSurface.startHandle.setPosition (map.x, map.y, rect.height);
            selectionHandleSurface.endHandle.setPosition (map.x, map.y, rect.height);

            currentlySelecting = true;
        } else {
            pendingCursorPosition = editor.positionAt (mouse.x, mouse.y);
            console.log ("Pending positioning");
        }
    }

    onReleased: {
        currentlySelecting = false;

        if (state == "selection") {
            // If there is a selection, then we want to pop up the menu
            var map = mapToItem (scene.content, mouse.x, mouse.y);

            showContextMenu (map.x, map.y);
        }
    }

    onPositionChanged: {
        if (state != "selection") {
            return;
        }

        selectionEnd = editor.positionAt (mouse.x, mouse.y);
        editor.select (selectionStart, selectionEnd);

        var rect = editor.positionToRectangle (selectionEnd);
        var map = mapToItem (scene.content, rect.x, rect.y);

        selectionHandleSurface.endHandle.setPosition (map.x, map.y, rect.height);
    }

    onPressAndHold: {
        if (currentlySelecting == true) {
            return;
        }

        var map = mapToItem (scene.content, mouse.x, mouse.y);

        showContextMenu (map.x, map.y);
    }

    ClipboardContextMenu {
        id: clipboardContextMenu

        payload: editor
        onTriggered: {
            parent.state = "";
            editor.select (editor.cursorPosition, editor.cursorPosition);
            editor.cursorPosition = selectionStart;
            selectionStart = 0;
            selectionEnd = 0;
        }
    }

    SelectionHandleSurface {
        id: selectionHandleSurface

        editor: parent

        onClose: {
            parent.state = "";
            parent.editor.select (editor.cursorPosition, editor.cursorPosition);
            parent.editor.cursorPosition = selectionStart;
            selectionStart = 0;
            selectionEnd = 0;
        }
    }

    states: [
        State {
            name: "selection"
            PropertyChanges {
                target: selectionHandleSurface
                visible: true
            }
        }
    ]
}
