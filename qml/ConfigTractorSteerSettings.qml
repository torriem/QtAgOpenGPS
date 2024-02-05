import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Rectangle{
    id: configTractorSteerSettings
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
/*    Rectangle{
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
            SpinBoxCM{
                id: lightbarCmPerPixel
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                to: 15
                boundValue: settings.setDisplay_lightbarCmPerPixel
                onValueModified: settings.setDisplay_lightbarCmPerPixel = value
                editable: true
                text: utils.cm_unit() + " " + qsTr("per pixel","As in units per pixel")
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
            SpinBoxCM{
                id: linelengthSetting
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 200
                to: 5000
                boundValue: settings.setAB_lineLength
                onValueModified: settings.setAB_lineLength = value
                editable: true
                text: utils.cm_unit()
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
            width: 102
            height: 102
            radius: 0
            border.color: "black"
            IconButtonTransparent{
                anchors.fill: parent
                anchors.margins: 1
                radius: 0
                checkable: true
                isChecked: settings.setAS_isConstantContourOn
                icon.source: "/images/ContourOn.png"
                onCheckedChanged: settings.setAS_isConstantContourOn = checked
            }
            Text{
                width: 150
                wrapMode: Text.WordWrap
                text: qsTr("Constant Contour Recording")
                anchors.bottom: parent.top
                anchors.left: parent.left
            }
        }*/
            IconButtonColor{
                anchors.right: parent.right
                anchors.top:parent.top
                anchors.margins: 40
                icon.source: "/images/AutoSteerOn.png"
                checkable: true
                isChecked: settings.setAS_isAutoSteerAutoOn
                onCheckableChanged: settings.setAS_isAutoSteerAutoOn = checked
                text: qsTr("Steer Switch Control")
                font.pixelSize:15
                implicitWidth: 120
                implicitHeight: 150
            }

        //}
    //}
    Rectangle{
        id: linewidthrect
        anchors.left: parent.left
        anchors.top: nudgedistrect.bottom
        height: 150
        width: 250
        anchors.margins: 20
        color: "transparent"
        TextLine{
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
                to: 8
                boundValue: settings.setDisplay_lineWidth
                onValueModified: settings.setDisplay_lineWidth = value
                text: qsTr("pixels")
                editable: true
            }
        }
    }
    Rectangle{
        id: nudgedistrect
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: 100
        width: 350
        anchors.margins: 20
        color: "transparent"
        TextLine{
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
            SpinBoxCM{
                id: snapDistance
                anchors.top: parent.top
                anchors.topMargin: 25
                height: 50
                anchors.left: parent.right
                anchors.leftMargin: 10
                from: 0
                to: 1000
                boundValue: settings.setAS_snapDistance
                onValueModified: settings.setAS_snapDistance = value
                editable: true
                text: utils.cm_unit()
            }
        }
    }
    Rectangle{
        id: lineacqLAheadrect
        anchors.left: linewidthrect.right
        anchors.verticalCenter: linewidthrect.verticalCenter
        anchors.margins: 50
        height: 100
        width: 350
        color: "transparent"
        TextLine{
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
                to: 10
                boundValue: settings.setAS_guidanceLookAheadTime
                onValueModified: settings.setAS_guidanceLookAheadTime = value
                editable: true
                text: qsTr("Seconds")
            }
        }
    }
    RowLayout{
        id: safety
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 100
        height: 150
        Column{
            id: manualTurnsLimit
            height: 130
            width: 100
            TextLine{ text: qsTr("Manual Turns Limit")}
            Image{
                source: "/images/Config/con_VehicleFunctionSpeedLimit.png"
                width: parent.width
                height: 90
            }
            SpinBoxKM{
                from: 0
                to: 20
                editable: true
                boundValue: settings.setAS_functionSpeedLimit
                onValueModified: settings.setAS_functionSpeedLimit = value
            }
            TextLine{ text: qsTr(utils.speed_unit())}
        }

        Column{
            id: minSpeed
            width: 100
            height: 130
            TextLine{ text: qsTr("Min AutoSteer Speed")}
            Image{
                source: "/images/Config/ConV_MinAutoSteer.png"
                width: parent.width
                height: 90
            }
            SpinBoxKM{
                from: 0
                to: 50
                editable: true
                boundValue: settings.setAS_minSteerSpeed
                onValueModified: settings.setAS_minSteerSpeed = value
            }
            TextLine{ text: qsTr(utils.speed_unit())}
        }
        Column{
            id: maxSpeed
            width: 100
            height: 130
            TextLine{ text: qsTr("Max AutoSteer Speed")}
            Image{
                source: "/images/Config/ConV_MaxAutoSteer.png"
                width: parent.width
                height: 90
            }
            SpinBoxCM{
                from: 0
                to: 50
                editable: true
                boundValue: settings.setAS_maxSteerSpeed
                onValueModified: settings.setAS_maxSteerSpeed = value
            }
            TextLine{ text: qsTr(utils.speed_unit())}
        }
        Column{
            id: maxTurnRate
            width: 100
            height: 130
            TextLine{ text: qsTr("Max Turn Rate")}
            Image{
                source: "/images/Config/ConV_MaxAngVel.png"
                width: parent.width
                height: 90
            }

            //The from and to values are deg/sce, but the final value output is in radians always
            SpinBoxCustomized {
                id: spinner
                from: 5
                to: 100
                editable: true
                value: utils.radians_to_deg(settings.setVehicle_maxAngularVelocity) // should be in radians!
                onValueChanged: settings.setVehicle_maxAngularVelocity = utils.deg_to_radians(value)
                //stepSize: spinBoxCM.stepSize
            }
            TextLine{ text: qsTr("Degrees/sec")}
        }
    }
}
