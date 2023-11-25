import QtQuick 2.0

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Rectangle{
            id:antennaType
            width: childrenRect.width+10
            height: childrenRect.height+ 10
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: "ghostwhite"
            border.color: "black"
            visible: true

            Text {
                id: text
                text: qsTr("Antenna Type")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row{
                anchors.top: text.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Rectangle{
                    Text{
                        text: qsTr("Dual")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: dual
                    IconButtonTransparent{
                        icon: "qrc:/images/Config/Con_SourcesGPSDual.png"
                        anchors.fill: parent
                        colorHover1: "transparent"
                        checkable: true
                        /*onClicked: {
                        harvester.color = "green"
                        tractor.color = "white"
                        tr4wd.color = "white"
                    }*/
                    }
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: fix
                    Text{
                        text: qsTr("Fix")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    IconButtonTransparent{
                        anchors.fill:parent
                        icon: "qrc:/images/Config/Con_SourcesGPSSingle.png"
                        colorHover1: "transparent"
                        checkable: true
                        /* onClicked: {
                        harvester.color = "white"
                        tractor.color = "green"
                        tr4wd.color = "white"
                    }*/
                    }
                }
            }
        }

        Rectangle{
            id:rtkAlarm
            width: childrenRect.width+10
            height: childrenRect.height+ 10
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "ghostwhite"
            visible: true

            Text {
                id: textRTK
                text: qsTr("RTK Alarm")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row{
                anchors.top: textRTK.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: alarm
                    IconButtonTransparent{
                        icon: "qrc:/images/Config/Con_SourcesRTKAlarm.png"
                        anchors.fill: parent
                        colorHover1: "transparent"
                        checkable: true
                        /*onClicked: {
                        harvester.color = "green"
                        tractor.color = "white"
                        tr4wd.color = "white"
                    }*/
                    }
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: killAutoSteer
                    IconButtonTransparent{
                        anchors.fill:parent
                        icon: "qrc:/images/AutoSteerOff.png"
                        colorHover1: "transparent"
                        checkable: true
                        /* onClicked: {
                        harvester.color = "white"
                        tractor.color = "green"
                        tr4wd.color = "white"
                    }*/
                    }
                }
            }
        }
    }
}
