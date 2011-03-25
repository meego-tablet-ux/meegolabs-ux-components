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

    property variant selectedDate :today()
    property variant daysOfWeek: [ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" ]
    property variant shortMonths: [ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" ]
    property variant fullMonths: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August',
        'September', 'October', 'November', 'December']

    property int month
    property int day
    property int year
    property int todayIndex

    signal dateSelected(variant date)

    function createDate(y,m,d) {
        var dateVal = new Date(y,m,d);
        return dateVal;
    }

    function today()
    {
        var currentDate = new Date()
        month = currentDate.getMonth()
        day = currentDate.getDate()
        year = currentDate.getFullYear()
        console.log("currentDate="+currentDate.toDateString());
        return currentDate
    }

    function isCurrentDate( date )
    {
        var retVal;
        var currentDate = new Date()
        if ((date.getDate() == currentDate.getDate()) && (date.getMonth() == currentDate.getMonth()) && (date.getFullYear() == currentDate.getFullYear())) {
            retVal = true
        }

        else
            retVal = false;

        //console.log("!!!!!!!!!!!!!!!! received date = "+ date.toString()+", current date = "+currentDate.toString()+" returning "+retVal);
        return retVal;

    }

    function startDay ( mm, yyyy ) {
        var firstDay = new Date( yyyy, mm, 1, 0, 0, 0, 0)
        return firstDay.getDay()
    }

    function daysInMonth(mm, yyyy) {
        return 32 - new Date(yyyy, mm, 32).getDate();
    }

    function isSelectedDate(dd, mm, yyyy) {
        return ( selectedDate.getFullYear() == yyyy &&
                  selectedDate.getMonth() == mm &&
                  selectedDate.getDate() == dd )
    }

    function nextMonth(refDate) {
        var newDate
        if (refDate.getMonth() == 11)
            newDate = new Date(refDate.getFullYear() + 1, 0, refDate.getDate())
        else
            newDate = new Date(refDate.getFullYear(), refDate.getMonth() + 1, refDate.getDate())
        return newDate
    }

    function prevMonth(refDate) {
        var newDate
        if (refDate.getMonth() == 1)
            newDate = new Date(refDate.getFullYear() - 1, 11, refDate.getDate())
        else
            newDate = new Date(refDate.getFullYear(), refDate.getMonth() - 1, refDate.getDate())
        return newDate
    }

    function getShortMonth(index) {
        var monName = new String(outer.shortMonths[index]) ;
        return monName;
    }

    function getTagValue(type,index) {
        console.log("type="+type+", index="+index);
        var val;
        if(type ==1) {
            val = dModel.get(index).tag;
        } else if(type==2) {
            val = mModel.get(index).tag;
        } else if(type==3) {
            val = yModel.get(index).tag;
        }
        return val;
    }

    function updateSelectedDate(d,m,y) {
        var tempDate = selectedDate;
        tempDate.setDate(d);
        tempDate.setMonth(m);
        tempDate.setFullYear(y);
        selectedDate = tempDate;
    }

    anchors.fill: parent
    clip:  false

    Column {
        id: stacker

        Item {
            id: titleBox
            width: outer.width
            height:50
            Text {
                id: prompt
                anchors.centerIn: parent
                text: qsTr("Due date")
                font.pixelSize: theme_fontPixelSizeLarge
                color:theme_fontColorNormal
            } // prompt text
        }//end titleBox

        Image {
            id:titleDivider
            width: parent.width
            source: "image://theme/menu_item_separator"
        } //end of titleDivider

        Item {
            id: dateSpinnersBox
            width:outer.width
            height: 130

            Row {
                id: popupRow
                anchors.centerIn: parent
                width: parent.width-20
                height:parent.height-20

                z: 10
                spacing: 10

                PopupList {
                    id: dayButton
                    width:  ( parent.width - 20 ) / 3
                    height: parent.height
                    popupListModel: dModel
                    value: day
                    onValueSelected: {
                        var d = (index+1)
                        var m = selectedDate.getMonth()
                        var y = selectedDate.getFullYear()
                        updateSelectedDate(d, m, y)
                    }
                }
                PopupList {
                    id: monthButton
                    width:  ( parent.width - 20 ) / 3
                    height: parent.height
                    popupListModel: mModel
                    value:shortMonths[month]
                    onValueSelected: {
                        var d = selectedDate.getDate()
                        var m = index
                        var y = selectedDate.getFullYear()
                        updateSelectedDate(d, m, y)
                    }
                }
                PopupList {
                    id: yearButton
                    width:  ( parent.width - 20 ) / 3
                    height: parent.height
                    popupListModel: yModel
                    value:year
                    onValueSelected: {
                        var d = selectedDate.getDate()
                        var m = selectedDate.getMonth()
                        var y = getTagValue(3,index)
                        updateSelectedDate(d, m, y)
                    }
                }

            } // date popups
        }//end date spinners box

        Item {
            id:calendarBox
            height: outer.height - (titleBox.height+dateSpinnersBox.height+buttonBox.height)
            width:outer.width

            Item {
                id: calendarView

                BorderImage {
                    id:calBg
                    anchors.fill: parent
                    source:"image://theme/notificationBox_bg"
                }

                property variant calendarShown:today() // points to a date for the currently shown calendar

                height: parent.height-20
                width: parent.width-20
                anchors.centerIn: parent

                Column {
                    id:calDataStacker
                    width:parent.width
                    height:parent.height

                    Item {
                        id: monthHeader
                        width:parent.width
                        height: 40

                        Image {
                            id: prevArrow
                            anchors.left: parent.left
                            anchors.leftMargin: 10;
                            anchors.top:  parent.top
                            anchors.verticalCenter: parent.verticalCenter
                            source: "image://theme/arrow-left"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: { calendarView.calendarShown = prevMonth(calendarView.calendarShown); selectedDate = calendarView.calendarShown;  }
                            }
                        }//prevArrow

                        Text {
                            id: monthAndYear
                            text: fullMonths[calendarView.calendarShown.getMonth()] + " " + calendarView.calendarShown.getFullYear()
                            font.pixelSize: theme_fontPixelSizeLarge; verticalAlignment: "AlignVCenter"; horizontalAlignment: "AlignHCenter"
                            anchors.centerIn: parent
                        }

                        Image {
                            id: nextArrow
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.top:  parent.top
                            anchors.verticalCenter: parent.verticalCenter
                            source: "image://theme/arrow-right"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: { calendarView.calendarShown = nextMonth(calendarView.calendarShown); selectedDate = calendarView.calendarShown;  }
                            }
                        }//nextArrow

                    } // month-year header

                    Image {
                        id:monthDivider
                        width: parent.width
                        source: "image://theme/menu_item_separator"
                    } //end of monthDivider


                    Item {
                        id: dayLabel
                        width: calendarView.width
                        height: daysGrid.height

                        Grid {
                            id: daysGrid

                            property int cellFontSize;

                            //font size is critical here because of little space, so reduce it if necessary
                            cellFontSize: if( theme_fontPixelSizeMedium > calendarView.width/daysGrid.columns * 0.4 ){
                                calendarView.width/daysGrid.columns * 0.4
                            }else{
                                theme_fontPixelSizeMedium
                            }

                            anchors { left: parent.left; top: parent.top; right: parent.right }
                            rows: 1; columns: 7; spacing: 0

                            Repeater {
                                model: daysOfWeek
                                Text {
                                    id: daysText
                                    text: daysOfWeek[index]
                                    horizontalAlignment: "AlignHCenter";
                                    font.pixelSize: daysGrid.cellFontSize
                                    width: calendarView.width/daysGrid.columns
                                }
                            }
                        }//edn grid
                    } // dayLabel

                    Image {
                        id:dayDivider
                        width: parent.width
                        source: "image://theme/menu_item_separator"
                    } //end of dayDivider


                    Grid {
                        id: calendarGrid

                        property real cellGridWidth: width / columns
                        property real cellGridHeight: height / rows
                        property int cellFontSize;

                        //font size is critical here because of little space, so reduce it if necessary
                        cellFontSize: if( theme_fontPixelSizeLarge > cellGridHeight * 0.9 ){
                            cellGridHeight * 0.9
                        }else{
                            theme_fontPixelSizeLarge
                        }

                        width: calendarView.width;
                        height:  parent.height - (dayLabel.height+monthHeader.height+monthDivider.height+dayDivider.height)
                        x: ( width - childrenRect.width ) * 0.5
                        rows: 6; columns: 7; spacing: 0

                        Repeater {
                            model: 42
                            Rectangle {
                                property bool doTag: isSelectedDate( calendarGrid.indexToDay(index),
                                                                     calendarView.calendarShown.getMonth(),
                                                                     calendarView.calendarShown.getFullYear())
                                property bool isToday: isCurrentDate( createDate( calendarGrid.indexToDay(index),
                                                                                 calendarView.calendarShown.getMonth(),
                                                                                 calendarView.calendarShown.getFullYear()))

                                border.width: (calendarGrid.indexToDay(index) == -1)? 1 : (doTag ? 3 : 1)
                                border.color: isToday ? theme_fontColorHighlight: theme_fontColorInactive

                                width: calendarGrid.cellGridWidth - (doTag ? 1 : 0)
                                height: calendarGrid.cellGridHeight - (doTag ? 1 : 0)
                                color: (doTag)? "steelblue" :((calendarGrid.indexToDay(index) == -1) ? "lightgray" : "white")
                                opacity:  (calendarGrid.indexToDay(index) == -1) ? 0.25 : 1
                                Text { text: calendarGrid.indexToDay(index)
                                    font.pixelSize: calendarGrid.cellFontSize    //theme_fontPixelSizeLarge
                                    anchors.centerIn: parent
                                    visible: (calendarGrid.indexToDay(index) != -1)
                                    color: isToday ? theme_fontColorHighlight: theme_fontColorNormal
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        var d = calendarGrid.indexToDay(index)
                                        if (d > 0) {
                                            var tempDate = selectedDate
                                            tempDate.setDate(d)
                                            tempDate.setMonth(calendarView.calendarShown.getMonth())
                                            tempDate.setFullYear(calendarView.calendarShown.getFullYear())
                                            selectedDate = tempDate
                                            console.log("selectedDate="+selectedDate.toString())
                                        }
                                    }
                                }
                            }
                        }

                        function indexToDay(index) {
                            var firstDay = startDay(calendarView.calendarShown.getMonth(), calendarView.calendarShown.getFullYear())
                            var dayCount = daysInMonth(calendarView.calendarShown.getMonth(), calendarView.calendarShown.getFullYear())

                            if (index < firstDay) return -1
                            if (index >= firstDay + dayCount) return -1

                            return (index+1) - firstDay
                        }
                    }//end grid
                }//end column calDataStacker

            } // calendarView

        }//end calendarBox

        Item {
            id: buttonBox
            width: outer.width
            height: buttonsStacker.height

            Column {
                id: buttonsStacker
                height: todayBox.height+buttonDivider1.height+buttonDivider2.height+footer.height
                width:parent.width
                Image {
                    id:buttonDivider1
                    width: parent.width
                    source: "image://theme/menu_item_separator"
                } //end of buttonDivider1

                Item {
                    id:todayBox
                    height:40
                    width: parent.width
                    opacity: (enabled)?1:0.5
                    Text {
                        id:todayButton
                        text: qsTr("Go to today's date")
                        anchors.centerIn: parent
                        font.pixelSize: theme_fontPixelSizeLarge
                        color:theme_fontColorHighlight
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { selectedDate = today() }
                    }
                }//end todaybox

                Image {
                    id:buttonDivider2
                    width: parent.width
                    source: "image://theme/menu_item_separator"
                } //end of buttonDivider2

                Item {
                    id: footer
                    height: 60
                    width: parent.width

                    Row {
                        id:okCancelRow
                        width:3*(parent.width/4)
                        height: 40
                        anchors.centerIn: parent
                        spacing: 10
                        Button {
                            id: okButton;
                            title: qsTr("OK");
                            height:parent.height
                            width:(parent.width/2)-20
                            bgSourceUp: "image://theme/btn_blue_up"
                            bgSourceDn: "image://theme/btn_blue_dn"
                            onClicked: {
                                // return the date selected
                                dateSelected(selectedDate)
                            }
                        } // OK

                        Button {
                            id: cancelButton
                            title: qsTr("Cancel")
                            enabled: true
                            height:parent.height
                            width:(parent.width/2)-20
                            bgSourceUp: "image://theme/btn_grey_up"
                            bgSourceDn: "image://theme/btn_grey_dn"
                            onClicked: {
                                dateSelected(undefined)
                            }
                        } // cancel
                    }//end row

                } // footer

            }//end column
        }//end buttonBox
    }//end stacker



    function setDays() {
        if (dModel.count != daysInMonth(selectedDate.getMonth(), selectedDate.getFullYear())) {

            dModel.clear(); // need to clear first since model changes with each month
            for (var i=0 ; i < daysInMonth(selectedDate.getMonth(), selectedDate.getFullYear()); i++) {
               dModel.append( { "tag": i+1 } );
            }
        }
    }

    function setMonths() {
        for (var i=0 ; i < shortMonths.length; i++) {
            mModel.append( { "tag": shortMonths[i] } );
        }
    }

    function setYears() {
        for (var i=1980 ; i < 2020; i++) {
            yModel.append( { "tag": i } );
        }
    }

    ListModel {
        id: dModel
        Component.onCompleted: {
            setDays();
        }
    }

    ListModel {
        id: mModel
        Component.onCompleted: {
            setMonths();
        }
    }

    ListModel {
        id: yModel
        Component.onCompleted: {
            setYears();
        }
    }

    // need a placeholder to allow the real models to be generated and assigned later.
    ListModel {
        id: dummyListModel
        ListElement { tag: "0" }
    }

    onSelectedDateChanged: {
        // fixup all values:
        setDays();
        calendarView.calendarShown = selectedDate
        todayButton.enabled = ! isCurrentDate(selectedDate)
        dayButton.value = (selectedDate.getDate()).toString()
        monthButton.value = shortMonths[selectedDate.getMonth()];
        yearButton.value = (selectedDate.getFullYear()).toString()
        yearButton.reInit();
        monthButton.reInit();
        dayButton.reInit();
    }

    Component.onCompleted: {
        selectedDate = today()
    }
}
