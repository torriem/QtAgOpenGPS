import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.right: parent.left
            width: 100
            id: subMenuTractor
            visible: true
            color: "white"
            height: childrenRect.height
            z: 15
            Column{
                visible: true
                anchors.fill: parent
                IconButton{
                    id: configTrSettings
                    icon: "qrc:/images/Config/ConS_VehicleConfig.png"
                    /* function toggle_(){
                    if(configImplement.visible == true){
                        configImplement.visible = false
                    }else{
                        configImplement.visible = true
                    }
                }

                onClicked: toggle_configImplement()*/
                }
                IconButton{
                    id: configTrDim
                    icon: "qrc:/images/Config/ConS_ImplementHitch.png"
                    function toggle_trdimwin(){
                        if(configTrDimWin.visible == true){
                            configTrDimWin.visible = false
                        }else{
                            configTrDimWin.visible = true
                        }
                    }

                    onClicked: toggle_trdimwin()
                }
                IconButton{
                    id: btnconfigTrAntDim
                    icon: "qrc:/images/Config/ConS_ImplementAntenna.png"
                    function toggle_trantdim(){
                        if(configTrAntWin.visible == true){
                            configTrAntWin.visible = false
                        }else{
                            configTrAntWin.visible = true
                        }
                    }

                    onClicked: toggle_trantdim()
                }
                IconButton{
                    id: btnconfigTrSteerSett
                    icon: "qrc:/images/Config/ConS_ModulesSteer.png"
                    function toggle_trantdim(){
                        if(configTrSteer.visible == true){
                            configTrSteer.visible = false
                        }else{
                            configTrSteer.visible = true
                        }
                    }

                    onClicked: toggle_trantdim()

                }
            }
        }

        Rectangle{
            anchors.fill: parent
            visible: true
            color: "ghostwhite"

            Text {
                id: text
                text: qsTr("Choose Vehicle Type")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row{
                anchors.top: text.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: harvester
                    IconButtonTransparent{
                        icon: "qrc:/images/vehiclePageHarvester.png"
                        anchors.fill: parent
                        colorHover1: "transparent"
                        checkable: true
                        onClicked: {
                            harvester.color = "green"
                            tractor.color = "white"
                            tr4wd.color = "white"
                        }
                    }
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: tractor
                    IconButtonTransparent{
                        anchors.fill:parent
                        icon: "qrc:/images/vehiclePageTractor.png"
                        colorHover1: "transparent"
                        checkable: true
                        onClicked: {
                            harvester.color = "white"
                            tractor.color = "green"
                            tr4wd.color = "white"
                        }
                    }
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: tr4wd
                    IconButtonTransparent{
                        anchors.fill: parent
                        icon: "qrc:/images/vehiclePage4WD.png"
                        colorHover1: "transparent"
                        checkable: true
                        onClicked: {
                            harvester.color = "white"
                            tractor.color = "white"
                            tr4wd.color = "green"
                        }
                    }
                }
            }
            Text {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("I consider the rest of this page nonessential for now and will be added at a later date")
            }
        }
    }
    Rectangle{
        id: configTrDimWin
        anchors.fill: parent
        color: "ghostwhite"

        visible: false
        Image {
            id: dimImage
            source: "qrc:/images/RadiusWheelBase.png"
            width: 350
            height: 350
            anchors.centerIn: parent
        }
        SpinBox{
            id: wheelBase
            anchors.verticalCenter: dimImage.verticalCenter
            anchors.right: dimImage.left
            from: 20
            value: 20
            to: 787
            editable: true
            Text{
                anchors.bottom: parent.top
                anchors.left: parent.left
                text: qsTr("Wheelbase")
            }
        }
        SpinBox{
            id: track
            anchors.top: dimImage.top
            anchors.left: dimImage.right
            from: 20
            value: 20
            to: 3937
            editable: true
            Text{
                anchors.bottom: parent.top
                anchors.left: parent.left
                text: qsTr("Track")
            }
        }
        SpinBox{
            id: turnRadius
            anchors.bottom: dimImage.bottom
            anchors.left: dimImage.right
            from: 20
            value: 20
            to: 787
            editable: true
            Text{
                anchors.bottom: parent.top
                anchors.left: parent.left
                text: qsTr("Turn Radius")
            }
        }
    }
    Rectangle{
        id: configTrAntWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image {
            id: antImage
            source: "qrc:/images/AntennaTractor.png"
            width: 350
            height: 350
            anchors.centerIn: parent
        }
        SpinBox{
            id: antForward
            anchors.bottom: antImage.top
            anchors.left: antImage.left
            anchors.leftMargin: 30
            from: -999
            value: 20
            to: 999
            editable: true
        }
        SpinBox{
            id: antHeight
            anchors.top: antImage.top
            anchors.topMargin: 100
            anchors.left: antImage.right
            anchors.leftMargin: -50
            from: 0
            value: 20
            to: 1000
            editable: true
        }
        SpinBox{
            id: antOffset
            anchors.bottom: antImage.bottom
            anchors.bottomMargin: 70
            anchors.left: antImage.right
            anchors.leftMargin: -25
            from: -500
            value: 20
            to: 500
            editable: true
        }
    }
    Rectangle{
        id: configTrSteer
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
                source: "qrc:/images/Config/ConV_CmPixel.png"
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
                source: "qrc:/images/Config/ConV_ABLineLength.png"
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
                    icon: "qrc:/images/ContourOn.png"
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
                    icon: "qrc:/images/AutoSteerOn.png"
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
                source: "qrc:/images/Config/ConV_LineWith.png"
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
                source: "qrc:/images/Config/ConV_SnapDistance.png"
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
                source: "qrc:/images/Config/ConV_GuidanceLookAhead.png"
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
}
