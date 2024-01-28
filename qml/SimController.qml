import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    color: "gray"
    height: 60
    width: childrenRect.width + 10
    Connections{
        target: settings
        function onSetMenu_isSimulatorOnChanged(){
            simBarRect.visible = settings.setMenu_isSimulatorOn
        }
    }

    Row{
        spacing: 4
        width: childrenRect.width
        height: 50
        anchors.centerIn: parent
        Button{
            text: "Reset"
            height: parent.height
            width: parent.height + 15
            onClicked: console.log("nothing doing")
        }
        Button{
            text: "wheelangle"
            font.pixelSize: 15
            height: parent.height
            width: parent.height + 15
            onClicked: steerSlider.value = 300
        }
        SliderCustomized {
            id: steerSlider
            objectName: "simSteer"
            multiplicationValue: 10
            from: 0
            to: 600
            value: 300
        }
        IconButtonTransparent{
            height: parent.height
            width: parent.height + 15
            icon.source: "/images/DnArrow64.png"
            onClicked: speedSlider.value -= 5
        }
        IconButtonTransparent{
            height: parent.height
            width: parent.height + 15
            icon.source: "/images/AutoStop.png"
            onClicked: speedSlider.value = 0
        }
        IconButtonTransparent{
            height: parent.height
            width: parent.height + 15
            icon.source: "/images/UpArrow64.png"
            onClicked: speedSlider.value += 5
        }
        IconButtonTransparent{
            height: parent.height
            width: parent.height + 15
            icon.source: "/images/YouTurn80.png"
            onClicked: console.log("nothing")
        }
    }
}