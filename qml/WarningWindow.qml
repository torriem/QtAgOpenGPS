import QtQuick 2.0

Rectangle{
    visible: true
    id: warningWindow
//    PropertyAnimation{
//        running: true
//        target: warningWindow
//        property: visible
//        to: visible = false
//        duration: 2000
//    }

    property string text: "Hello"
    /*function hideParent(){
        parent.visible = false
    }

    function js_hidewindow(){
        if(warningWindow.visible == true){
            setTimeout(hideParent, 1000)
        }
    }
    onVisibleChanged: js_hidewindow()*/
    color: "red"
    border.color: "black"
    border.width: 2
    width: 400
    height: childrenRect.height + 20
    Text{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        text: warningWindow.text
    }
}
