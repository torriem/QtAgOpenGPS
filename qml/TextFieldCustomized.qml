import QtQuick 2.0
import QtQuick.Controls 2.5

Button{
    //property alias textFieldCustomized: textFieldCustomized
    implicitWidth: 75
    implicitHeight: 35
    contentItem: Rectangle{
        id: textFieldCustomized_content
        Text{
            anchors.centerIn: parent
            //text: textFieldCustomized.text
            text: "blah"
            z: 1
        }
    }

    background: Rectangle{
        border.width: 1
        border.color: "black"
        color: "white"
    }
    onClicked: {
        console.log("clicked opening window")
        numberInputWindow.visible = true
    }
    NumberInputWindow{
        id: numberInputWindow
        z: 10
       visible: false
    }

//    Rectangle{
//        anchors.top: mainWindow.top
//        anchors.horizontalCenter: mainWindow.horizontalCenter
//        id: textFieldCustomized
//        visible: false
//        implicitHeight: 100
//        implicitWidth: 150
//        border.color: "black"
//        color: "lightgray"
//        border.width: 2
//        TextField{
//            property int bottomNum: 0
//            property int topNum: 3000
//            implicitWidth: 100
//            implicitHeight: 40
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: parent.top
//            anchors.topMargin: 10
//            background: Rectangle{
//                border.color: "black"
//                border.width: 1
//                color: "white"
//            }
//            //    validator: IntValidator{
//            //        function reject(){
//            //            if(textFieldCustomized.text < 300){
//            //                textFieldCustomized.acceptableInput = false
//            //                console.log("rejected")
//            //            }else{
//            //                console.log("accepted")
//            //            }
//            //        }
//            //    }
//        }
//    }
}
