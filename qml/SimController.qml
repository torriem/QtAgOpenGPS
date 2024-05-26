import QtQuick 2.0
import QtQuick.Controls.Fusion
import "components"

Rectangle{
    color: boundaryInterface.isOutOfBounds ? "darksalmon" : "gray"
    height: 60 * theme.scaleHeight
    width: 650 * theme.scaleWidth
	function changedSteerDir(isRight){
		if(isRight){
			steerSlider.value = steerSlider.value + 10
		}else{
			steerSlider.value = steerSlider.value - 10
		}
	}
    Connections{
        target: settings
        function onSetMenu_isSimulatorOnChanged(){
            simBarRect.visible = settings.setMenu_isSimulatorOn
        }
    }

    Row{
        spacing: 4 * theme.scaleWidth
        width: childrenRect.width
        height: 50 * theme.scaleHeight
        anchors.centerIn: parent
        Button{
			id: resetButton
            text: "Reset"
            font.pixelSize: 15
            height: parent.height
            width: 65 * theme.scaleWidth
            onClicked: aog.sim_reset()
        }
        Button{
            text: "wheelangle"
            font.pixelSize: 15
            height: parent.height
            width: 65 * theme.scaleWidth
            onClicked: steerSlider.value = 300
        }
        SliderCustomized {
            id: steerSlider
            objectName: "simSteer"
            multiplicationValue: 10
            height: 50 * theme.scaleHeight
			width: 200 * theme.scaleWidth
            from: 0
            to: 600
            value: 300
        }
        IconButtonTransparent{
            height: parent.height
            width: 65 * theme.scaleWidth
            icon.source: "/images/DnArrow64.png"
            onClicked: aog.sim_bump_speed(false)
        }
        IconButtonTransparent{
            height: parent.height
            width: 65 * theme.scaleWidth
            icon.source: "/images/AutoStop.png"
            onClicked: aog.sim_zero_speed()
        }
        IconButtonTransparent{
            height: parent.height
            width: 65 * theme.scaleWidth
            icon.source: "/images/UpArrow64.png"
            onClicked: aog.sim_bump_speed(true)
        }
        IconButtonTransparent{
            height: parent.height
            width: 65 * theme.scaleWidth
            icon.source: "/images/YouTurn80.png"
            onClicked: console.log("nothing")
        }
    }
}
