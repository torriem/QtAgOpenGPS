import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    Rectangle{
        id: steerConfigFirst
        color: "ghostwhite"
        border.color: "black"
        border.width: 8
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
        Rectangle{
            id:topLine
            color:"gray"
            visible: true
            width: parent.width-16
            height: 40
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Auto Steer Config")
            }

            Button {
                id: close
                text: qsTr("×")
                font.pixelSize: parent.height
                width: parent.height+3
                height:parent.height
                anchors.right: parent.right
            }
            Button {
                id: help
                text: qsTr("?")
                width: parent.height+3
                height:parent.height
                anchors.right: close.left
                anchors.rightMargin: 20
            }
        }
        Row{
            id: tabButtons
            anchors.top: topLine.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            spacing: 4
            anchors.right: childrenRect.right
            IconButton{
                icon.source: "qrc:/images/ST_SteerTab.png"
                function toggle_steertab(){
                    if(steerTab.visible == true){
                        steerTab.visible = false
                    }else{
                        steerTab.visible = true
                    }
                }
                onClicked: toggle_steertab
            }
            IconButton{
                icon.source: "qrc:/images/ST_GainTab.png"
                function toggle_gaintab(){
                    if(gainTab.visible == true){
                        gainTab.visible = false
                    }else{
                        gainTab.visible = true
                    }
                }
                onClicked: toggle_gaintab
            }
            IconButton{
                icon.source: "qrc:/images/Sf_StanleyTab.png"
                function toggle_stanleytab(){
                    if(stanleyTab.visible == true){
                        stanleyTab.visible = false
                    }else{
                        stanleyTab.visible = true
                    }
                }
                onClicked: toggle_stanleytab
            }
            IconButton{
                icon.source: "qrc:/images/ST_ppTab.png"
                function toggle_pptab(){
                    if(ppTab.visible == true){
                        ppTab.visible = false
                    }else{
                        ppTab.visible = true
                    }
                }
                onClicked: toggle_pptab
            }
        }
        Rectangle{
            id: steerTab
            anchors.top: tabButtons.bottom
            anchors.right: tabButtons.right
            anchors.left: tabButtons.left
            height: parent.height * .66
            visible: true
            color: "light gray"
        }
        Rectangle{
            id: gainTab
            anchors.top: tabButtons.bottom
            anchors.right: tabButtons.right
            anchors.left: tabButtons.left
            height: parent.height * .66
            visible: false
            color: "light gray"
        }
        Rectangle{
            id: stanleyTab
            anchors.top: tabButtons.bottom
            anchors.right: tabButtons.right
            anchors.left: tabButtons.left
            height: parent.height * .66
            visible: false
            color: "light gray"
        }
        Rectangle{
            id: ppTab
            anchors.top: tabButtons.bottom
            anchors.right: tabButtons.right
            anchors.left: tabButtons.left
            height: parent.height * .66
            visible: false
            color: "light gray"
        }
    }
}
