import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    z: 10
    id: numberInputWindow
    width: 250
    height: 150
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: 10
    border.color: "black"
    border.width: 2
    color: "lightgray"
    visible: true
        TextField{
            property int bottomNum: 0
            property int topNum: 3000
            implicitWidth: 100
            implicitHeight: 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            background: Rectangle{
                border.color: "black"
                border.width: 1
                color: "white"
            }
                validator: IntValidator{

//                    function reject(){
//                        if(textFieldCustomized.text < 300){
//                            textFieldCustomized.acceptableInput = false
//                            console.log("rejected")
//                        }else{
//                            console.log("accepted")
//                        }
//                    }
                }
        }
        IconButtonTransparent{
            id: cancel
            icon.source: "qrc:/images/Cancel64.png"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 10
            onClicked: parent.visible = false
        }
        IconButtonTransparent{
            objectName: "btnNumberWindowSave"
            icon.source: "qrc:/images/OK64.png"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 10
        }
}
