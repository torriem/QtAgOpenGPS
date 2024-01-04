import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    z: 5
    id: sectionWidth
    color: "transparent"
    height: 60
    width: 75
    property int titleText: 0
    property int value: 0
    property bool inputVisi: true
    property int bottomNum: 3
    property int topNum: 100

    function secwidthvisible(){
        if(numOfSections.value > titleText){
            spinner.visible = false
        }else{
            spinner.visible = true
        }
    }
    TextFieldCustomized{
        id: spinner
        anchors.bottom: parent.bottom
        text: middleRowSpinbox.value

        //value: sectionWidth.value
    }
    Text{
        text: sectionWidth.titleText
        anchors.bottom: spinner.top
        anchors.left: sectionWidth.left
    }
}
