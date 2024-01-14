import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractorSteerSettings
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    Rectangle{
        id: lightbarrect
        anchors.left: parent.left
        anchors.top: parent.top
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Text{
            id: lightbartitletxt
            text: qsTr("LightBar - Distance per pixel")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Image {
            id: lightbarimage
            source: "/images/Config/ConV_CmPixel.png"
            anchors.left: parent.left
            anchors.top: lightbartitletxt.bottom
            anchors.bottom: parent.bottom
            width: parent.width*.5
            SpinBoxCustomized{
                id: lightbarSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                value: settings.setDisplay_lightbarCmPerPixel
                onValueChanged: settings.setDisplay_lightbarCmPerPixel
                to: 15
                editable: true
                text: qsTr("in  /  pixel")
            }
        }
    }
    Rectangle{
        id: ablinelength
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Text{
            id: linelengthtitletxt
            text: qsTr("AB Line Length")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Image {
            id: linelengthimage
            source: "/images/Config/ConV_ABLineLength.png"
            anchors.left: parent.left
            anchors.top: linelengthtitletxt.bottom
            anchors.bottom: parent.bottom
            width: parent.width*.5
            SpinBoxCustomized{
                id: linelengthSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 200
                value: settings.setAB_lineLength
                onValueChanged: settings.setAB_lineLength
                to: 5000
                text: qsTr("cm")
            }
        }
    }
    Rectangle{
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: 100
            height: 100
            radius: 5
            border.color: "black"
            IconButtonTransparent{
                anchors.fill: parent
                icon.source: "/images/ContourOn.png"
            }
            Text{
                text: qsTr("Constant Contour
Recording")
                anchors.bottom: parent.top
                anchors.left: parent.left
            }
        }
        Rectangle{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            width: 100
            height: 100
            radius: 5
            border.color: "black"
            IconButtonTransparent{
                anchors.fill: parent
                icon.source: "/images/AutoSteerOn.png"
            }
            Text{
                text: qsTr("Steer Switch Control")
                anchors.bottom: parent.top
                anchors.left: parent.left
            }
        }

    }
    Rectangle{
        id: linewidthrect
        anchors.right: parent.right
        anchors.top: parent.top
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Text{
            id: linewidthtitletxt
            text: qsTr("Line Width")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Image {
            id: linewidthimage
            source: "/images/Config/ConV_LineWith.png"
            anchors.left: parent.left
            anchors.top: linewidthtitletxt.bottom
            anchors.bottom: parent.bottom
            width: parent.width*.5
            SpinBoxCustomized{
                id: linewidthSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 1
                value: settings.setDisplay_lineWidth
                onValueChanged: settings.setDisplay_lineWidth
                to: 8
                text: qsTr("pixels")
            }
        }
    }
    Rectangle{
        id: nudgedistrect
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Text{
            id: nudgedisttitletxt
            text: qsTr("Nudge Distance")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Image {
            id: nudgedistimage
            source: "/images/Config/ConV_SnapDistance.png"
            anchors.left: parent.left
            anchors.top: nudgedisttitletxt.bottom
            anchors.bottom: parent.bottom
            width: parent.width*.5
            SpinBoxOneDecimal{
                id: nudgedistSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                value: settings.setAS_snapDistance
                onValueChanged: settings.setAS_snapDistance
                to: 1000
                editable: true
                    text: qsTr("cm")
            }
        }
    }
    Rectangle{
        id: lineacqLAheadrect
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 150
        width: 400
        anchors.margins: 20
        color: "transparent"
        Text{
            id: lineacqLAheadtitletxt
            text: qsTr("Line Acquire Look Ahead")
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Image {
            id: lineacqLAheadimage
            source: "/images/Config/ConV_GuidanceLookAhead.png"
            anchors.left: parent.left
            anchors.top: lineacqLAheadtitletxt.bottom
            anchors.bottom: parent.bottom
            width: parent.width*.5
            SpinBoxOneDecimal{
                id: lineacqLAheadSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: .1
                value: settings.setAS_guidanceLookAheadTime
                onValueChanged: settings.setAS_guidanceLookAheadTime
                to: 10
                text: qsTr("Seconds")
            }
        }
    }
}
