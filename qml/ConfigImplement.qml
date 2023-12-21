import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 200
            anchors.right: parent.left
            width: childrenRect.width
            id: subMenuImplement
            visible: true
            color: "white"
            height: childrenRect.height
            Column{
                visible: true
                width: childrenRect.width
                anchors.right: parent.right
                anchors.top: parent.top
                IconButton{
                    id: configImpSettings
                    icon.source: "qrc:/images/Config/ConS_VehicleConfig.png"
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
                    id: configImpDim
                    icon.source: "qrc:/images/Config/ConS_ImplementHitch.png"
                    function toggle_impdimwin(){
                        if(configImpDimWin.visible == true){
                            configImpDimWin.visible = false
                        }else{
                            configImpDimWin.visible = true
                        }
                    }

                    onClicked: toggle_impdimwin()
                }
                IconButton{
                    id: configImpSection
                    icon.source: "qrc:/images/Config/ConS_ImplementSection.png"
                    function toggle_impsecwin(){
                        if(configImpSecWin.visible == true){
                            configImpSecWin.visible = false
                        }else{
                            configImpSecWin.visible = true
                        }
                    }

                    onClicked: toggle_impsecwin()
                }
                IconButton{
                    id: configImpTiming
                    icon.source: "qrc:/images/Config/ConS_ImplementSettings.png"
                    function toggle_imptimwin(){
                        if(configImpTimWin.visible == true){
                            configImpTimWin.visible = false
                        }else{
                            configImpTimWin.visible = true
                        }
                    }

                    onClicked: toggle_imptimwin()

                }
                IconButton{
                    id: configImpSwitches
                    icon.source: "qrc:/images/Config/ConS_ImplementSwitch.png"
                    function toggle_impswitchwin(){
                        if(configImpSwitchWin.visible == true){
                            configImpSwitchWin.visible = false
                        }else{
                            configImpSwitchWin.visible = true
                        }
                    }

                    onClicked: toggle_impswitchwin()

                }
            }
        }

        Text{
            id: text
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Attachment Style")
        }//I didn't bother with setting colors here, because IMOthe color should be set by the backend
        GridLayout{
            anchors.top: text.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            height: 400
            rows:2
            columns: 2
            flow:Grid.TopToBottom
            Rectangle{
                width:200
                height:150
                radius:5
                id: i3pt
                //I didn't bother with setting colors here, because IMO the color should be set by the backend
                //I'll do that after the backend is caught up
                IconButtonTransparent{
                    anchors.fill: parent
                    icon.source: "qrc:/images/ToolChkRear.png"
                    colorHover1: "transparent"
                    checkable: true
                    /*property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                fieldTexture.color = "green"
                            }else{
                                fieldTexture.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                }
                color: "white"
                border.color: "black"
            }

            Rectangle{
                width:200
                height:150
                radius:5
                id: i3ptfront
                IconButtonTransparent{
                    anchors.fill: parent
                    icon.source: "qrc:/images/ToolChkFront.png"
                    colorHover1: "transparent"
                    checkable: true
                    property bool toggleBtn: false
                    /*onToggleBtnChanged: {
                            if(toggleBtn == true){
                                fieldTexture.color = "green"
                            }else{
                                fieldTexture.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                }
                color: "white"
                border.color: "black"

            }
            Rectangle{
                width:200
                height:150
                radius:5
                id: itrailed
                IconButtonTransparent{
                    anchors.fill: parent
                    icon.source: "qrc:/images/ToolChkTrailing.png"
                    colorHover1: "transparent"
                    checkable: true
                    /*property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                fieldTexture.color = "green"
                            }else{
                                fieldTexture.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                }
                color: "white"
                border.color: "black"

            }
            Rectangle{
                width:200
                height:150
                radius:5
                id: iTBT
                IconButtonTransparent{
                    anchors.fill: parent
                    icon.source: "qrc:/images/ToolChkTBT.png"
                    colorHover1: "transparent"
                    checkable: true
                    /*property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                fieldTexture.color = "green"
                            }else{
                                fieldTexture.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                }
                color: "white"
                border.color: "black"

            }
        }

    }
    Rectangle{
        id: configImpDimWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image{
            source: "qrc:/images/ToolHitchPageTrailing.png"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 15
            height: parent.height*.75
            SpinBox{
                anchors.bottom: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 500
                from: 4
                value : 1
                to:1181
                editable: true
            }
            SpinBox{
                anchors.bottom: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 50
                from: 4
                value : 1
                to:1181
                editable: true
            }
        }
    }
    Rectangle{
        id: configImpSecWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            text: qsTr("Page will be added after the backend is caught up, would be useless to now")
        }
    }
    Rectangle{
        id: configImpTimWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image{
            id: configImpImage
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 70
            anchors.rightMargin: 30
            anchors.leftMargin: 200
            anchors.topMargin: 100
            source: "qrc:/images/ImplementSettings.png"

            SpinBox{
                anchors.right: parent.left
                anchors.top: parent.top
                from: 0
                value: 1
                to: 22
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("On (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                from: 0
                value:0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Off (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.bottom: parent.bottom
                from: 0
                value: 0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Delay (secs)")
                }
            }
            SpinBox{
                anchors.left: parent.left
                anchors.leftMargin: 500
                anchors.top: parent.bottom
                anchors.bottomMargin: -10
                from: -984
                value: 0
                to: 984
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Offset")
                }
            }
            SpinBox{
                anchors.right: parent.right
                anchors.rightMargin: 120
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 100
                from: -1181
                value: 0
                to: 1181
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Overlap / Gap")
                }
            }
        }
    }
    Rectangle{
        id: configImpSwitchWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Rectangle{
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            width: workswitchcolumn.width + 40
            anchors.margins: 40
            border.color: "black"
            color: "ghostwhite"
            Text{
                id: workTitle
                anchors.top: parent.top
                anchors.left: parent.left
                text: qsTr("Work Switch")
            }
            ColumnLayout{
                id: workswitchcolumn
                anchors.top: workTitle.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: childrenRect.width
                anchors.margins: 15
                Button{
                    objectName: "btnWorkSwitch"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: workSwitchbuttonimage.right
                            text: qsTr("Work Switch")
                        }

                        Image {
                            id: workSwitchbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/HydraulicLiftOff.png"
                        }
                    }
                }

                Button{
                    objectName: "btnWorkSwitchManual"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: manualSectionsbuttonimage.right
                            text: qsTr("Manual Sections")
                        }

                        Image {
                            id: manualSectionsbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/ManualOff.png"
                        }
                    }
                }
                Button{
                    objectName: "btnWorkSwitchAuto"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: autoSectionsbuttonimage.right
                            text: qsTr("Auto Sections")
                        }

                        Image {
                            id: autoSectionsbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/SectionMasterOff.png"
                        }
                    }
                }
                Button{
                    objectName: "workSwitchstatebutton"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Image {
                            id: workSwitchStateImage
                            anchors.fill: parent
                            source: "qrc:/images/SwitchActiveClosed.png"
                        }
                    }
                }
            }
        }
        Rectangle{
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            width: steerswitchcolumn.width + 40
            anchors.margins: 40
            border.color: "black"
            color: "ghostwhite"
            Text{
                id: steerTitle
                anchors.top: parent.top
                anchors.left: parent.left
                text: qsTr("Steer Switch")
            }
            ColumnLayout{
                id: steerswitchcolumn
                anchors.top: steerTitle.bottom
                anchors.bottom: parent.bottom
                width: childrenRect.width
                anchors.right: parent.right
                anchors.margins: 15
                Button{
                    objectName: "btnSteerSwitch"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: steerSwitchbuttonimage.right
                            text: qsTr("Steer Switch")
                        }

                        Image {
                            id: steerSwitchbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/AutoSteerOff.png"
                        }
                    }
                }

                Button{
                    objectName: "btnSteerSwitchManual"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: steermanualSectionsbuttonimage.right
                            text: qsTr("Manual Sections")
                        }

                        Image {
                            id: steermanualSectionsbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/ManualOff.png"
                        }
                    }
                }
                Button{
                    objectName: "btnSteerSwitchAuto"
                    background: Rectangle{
                        color: parent.down ? "green" : "white"
                        border.color: "black"
                        implicitWidth: 300
                        implicitHeight: 90
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: steerautoSectionsbuttonimage.right
                            text: qsTr("Auto Sections")
                        }

                        Image {
                            id: steerautoSectionsbuttonimage
                            anchors.left: parent.left
                            anchors.top: parent.top
                            width: parent.height
                            height: parent.height
                            source: "qrc:/images/SectionMasterOff.png"
                        }
                    }
                }
            }
        }
    }
}
