// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
import QtQuick
import QtQuick.Controls.Fusion

//This is a the row of on-screen section-control buttonw

Rectangle {
    id: sectionButtons

    /*
    MockSettings { //for testing with qmlscene only
        id: settings
    }

    AOGInterface { //for testing with qmlscene only
        id: aog
    }*/

    width: 600
    height: childrenRect.height * theme.scaleHeight

    color: "transparent"

    property bool triState: true
    property int numSections: (settings.setTool_isSectionsNotZones ? settings.setVehicle_numSections : settings.setTool_zones[0])
    property var buttonState: [ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]
    property color offColor: "Crimson"
    property color offTextColor: "White"
    property color onColor: "DarkGoldenrod"
    property color onTextColor: "White"
    property color autoColor: "ForestGreen"
    property color autoTextColor: "White"

    signal sectionClicked(int whichsection)

    //methods
    function setColors() {
        //same colors for sections and zones
        if (settings.setDisplay_isDayMode) {
            sectionButtons.offColor = "Red"
            sectionButtons.offTextColor = "Black"

            sectionButtons.autoColor = "Lime"
            sectionButtons.autoTextColor = "Black"

            sectionButtons.onColor = "Yellow"
            sectionButtons.onTextColor = "Black"
        } else {
            sectionButtons.offColor = "Crimson"
            sectionButtons.offTextColor = "White"

            sectionButtons.autoColor = "ForestGreen"
            sectionButtons.autoTextColor = "White"

            sectionButtons.onColor = "DarkGoldenRod"
            sectionButtons.onTextColor = "White"
        }
    }

    function setSectionState (sectionNo: int, new_state: int) {
        //states: 0 = off, 1 = auto, 2 = on
        var temp1 = aog.sectionButtonState //actual sections or row units
        var temp2 = sectionButtons.buttonState //onscreen section button
        var j


        if (settings.setTool_isSectionsNotZones) {
            //1:1 correlation between buttons and sections
            temp1[sectionNo] = new_state //this is the tie-in back to the C++ land
            temp2[sectionNo] = new_state //this is the onscreen button
        } else {
            //zones, not buttons. numSections is the number of zones
            temp2[sectionNo] = new_state; //this is the onscreen button

            var zone_left
            var zone_right

            //get the left-most section in the zone
            if (sectionNo===0) zone_left = 0
            else zone_left = settings.setTool_zones[sectionNo]

            //get the right-most section in the zone
            zone_right = settings.setTool_zones[sectionNo+1]

            //set all sections between left and right
            for (j = zone_left ; j < zone_right ; j++) {
                temp1[j] = new_state //set individual sections button state (grouped together)
            }
        }
        aog.sectionButtonState = temp1
        sectionButtons.buttonState = temp2
    }

    function setAllSectionsToState(new_state: int) {
        //states: 0 = off, 1 = auto, 2 = on
        var temp1 = aog.sectionButtonState
        var temp2 = sectionButtons.buttonState
        var i,j

        if (settings.setTool_isSectionsNotZones) {
            //1:1 correlation between buttons and sections
            for (i=0; i < 65 ; i++) {
                if (i < numSections)
                    temp1[i] = new_state //this is the tie-in back to the C++ land
                else
                    temp1[i] = 0; //off

                if (i < 16)
                    temp2[i] = new_state //this is the onscreen button
            }
         } else {
            //zones, not buttons. numSections is the number of zones
            for (i=0; i < numSections; i++) {
                temp2[i] = new_state; //this is the onscreen button

                var zone_left
                var zone_right

                //get the left-most section in the zone
                if (i===0) zone_left = 0
                else zone_left = settings.setTool_zones[i]

                //get the right-most section in the zone
                zone_right = settings.setTool_zones[i+1]

                //set all sections between left and right
                for (j = zone_left ; j < zone_right ; j++) {
                    temp1[j] = new_state
                }
            }
        }
        aog.sectionButtonState = temp1
        sectionButtons.buttonState = temp2
   }

    onNumSectionsChanged: {
        buttonModel.clear()
        for (var i = 0; i < numSections; i++) {
            buttonModel.append( { sectionNo: i } )
        y}
    }

    //callbacks, connections, and signals
    Component.onCompleted:  {
        setColors()
        buttonModel.clear()
        for (var i = 0; i < numSections; i++) {
            buttonModel.append( { sectionNo: i } )
        }
    }

    Connections {
        target: settings
        function onSetDisplay_isDayModeChanged() {
            setColors()
        }
    }

    ListModel {
        id: buttonModel
        //this will hold the individual buttons
        //{ sectionNo: button_number }
    }

    Component {
        id: sectionViewDelegate
        SectionButton {
            width: (sectionButtons.width / numSections) > 40 ? (sectionButtons.width / numSections) : 40
            buttonText: (model.sectionNo + 1).toFixed(0)
            visible: (model.sectionNo < numSections) ? true : false
            color: (sectionButtons.buttonState[model.sectionNo]===0 ? offColor : (sectionButtons.buttonState[model.sectionNo] === 1 ? autoColor : onColor))
            textColor: (sectionButtons.buttonState[model.sectionNo]===0 ? offTextColor : (sectionButtons.buttonState[model.sectionNo] === 1 ? autoTextColor : onTextColor))

            onClicked: {
                //emit signal
                sectionClicked(model.sectionNo)
                //set the state here
                setSectionState(model.sectionNo, (sectionButtons.buttonState[model.sectionNo] + 1 ) % 3)
            }
        }
    }

    ListView {
        id: sectionButtonList
        orientation: Qt.Horizontal
        width: parent.width
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        model: buttonModel
        boundsMovement: Flickable.StopAtBounds

        ScrollBar.horizontal: ScrollBar {
            parent: sectionButtonList.parent
            anchors.top: sectionButtonList.bottom
            anchors.left: sectionButtonList.left
            anchors.right: sectionButtonList.right
        }

        clip: true

        delegate: sectionViewDelegate
        flickableDirection: Flickable.HorizontalFlick
    }
}
