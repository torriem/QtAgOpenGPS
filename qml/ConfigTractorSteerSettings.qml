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
            SpinBox{
                id: lightbarSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                value: 1
                to: 15
                editable: true
                Text {
                    text: qsTr("in  /  pixel")
                    anchors.top:parent.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                }
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
            SpinBox{
                id: linelengthSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 656
                value: 5249
                to: 16400
                editable: true
                Text {
                    text: qsTr("ft")
                    anchors.top:parent.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                }
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
            SpinBox{
                id: linewidthSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 1
                value: 2
                to: 8
                editable: true
                Text {
                    text: qsTr("pixels")
                    anchors.top:parent.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                }
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
            SpinBox{
                id: nudgedistSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                value: 3
                to: 155
                editable: true
                Text {
                    text: qsTr("in")
                    anchors.top:parent.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                }
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
            SpinBox{
                id: lineacqLAheadSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                value: 1
                to: 10
                editable: true
                Text {
                    text: qsTr("Seconds")
                    anchors.top:parent.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                }
            }
        }
    }
}
