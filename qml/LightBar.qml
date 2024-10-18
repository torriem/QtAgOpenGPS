import QtQuick
import QtQuick.Controls.Fusion
import 'components' as Comp

Rectangle {
    id: lightbar
    width: 640
    height: 40
    color: "transparent"

    //MockSettings {
    //    id: settings
    //}

    property color colorClose: "#80f34c"
    property color colorFar: "#f9804c"
    property color colorRight: "#00fa00"
    property color colorLeft: "#fa4c00"


    property double dotDistance: 0

    ListModel {
        id: dots

        //model is
        //{ x: offset, isRight: true, dotSize: size }
    }

    function calculateDots(){
        var numDots = 0 //integer
        var i = 0

        var limited_dotDistance = dotDistance

        var limit = settings.setDisplay_lightbarCmPerPixel * 8
        if (limited_dotDistance < -limit) limited_dotDistance = -limit
        if (limited_dotDistance > limit) limited_dotDistance = limit

        dots.clear()

        numDots = -limited_dotDistance / settings.setDisplay_lightbarCmPerPixel

        for (i = -8 ; i < 0 ; i ++ ) {
            if (numDots < 0 && i >= numDots) {
                //big dot
                dots.append( { x: (i*32) - 16, isRight: false, dotSize: 24.0 } )
            } else {
                //small dot
                dots.append( {x: (i*32) - 16, isRight: false, dotSize: 8 } )
            }
        }

        for (i = 1 ; i < 9 ; i++ ) {
            if (numDots > 0 && i <= numDots) {
                //big dot
                dots.append( { x: (i*32) + 16, isRight: true, dotSize: 24.0 } )
            } else {
                //small dot
                dots.append( {x: (i*32) + 16, isRight: true, dotSize: 8 } )
            }

        }
    }

    onDotDistanceChanged: {
        calculateDots()
    }

    Comp.OutlineText {
        id: number
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        font.pixelSize: 32

        //dotDistance is in cm.  convert to meters, then
        //convert to local units, print
        text: utils.cm_to_unit_string(Math.abs(dotDistance / 100), 0)

        color: (aog.avgPivDistance > 20) ?
                   colorFar :
                   colorClose
        z: 2
    }

    Repeater {
        id: dotDrawer
        model: dots
        delegate: Rectangle {
            color: "transparent"
            width: 32
            height: 32
            anchors.verticalCenter: parent.verticalCenter
            x: model.x + lightbar.width / 2 - 16

            Rectangle {
                width: model.dotSize
                height: model.dotSize
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                radius: model.dotSize / 2

                border.width: 2
                border.color: "black"
                color: model.isRight === true? colorRight : colorLeft
            }
        }
    }
}
