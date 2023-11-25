import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
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
                        icon: "qrc:/images/ToolChkRear.png"
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
                        icon: "qrc:/images/ToolChkFront.png"
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
                        icon: "qrc:/images/ToolChkTrailing.png"
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
                        icon: "qrc:/images/ToolChkTBT.png"
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
    }

