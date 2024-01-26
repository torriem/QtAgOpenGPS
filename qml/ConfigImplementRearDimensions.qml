import QtQuick 2.0
import QtQuick.Controls 2.5

/*todo:
//in the application output we get the warning from this page:
qrc:/qml/ConfigImplementDimensions.qml:26:9: QML SpinBoxCM: Binding loop detected for property "value"
qrc:/qml/ConfigImplementDimensions.qml:16:9: QML SpinBoxCM: Binding loop detected for property "value"
I'm not sure why, I think I did them the same as everywhere else.
*/
Rectangle{
    id: configImpDimWin
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    Image{
        source: "/images/ToolHitchPageRear.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 15
        height: parent.height*.75
        SpinBoxCM{
            id: hitchLength
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.rightMargin: parent.width * 0.6
            from: 10
            to:3000
            value: Number(settings.setVehicle_hitchLength) < 0 ? -Number(settings.setVehicle_hitchLength) : Number(settings.setVehicle_hitchLength)
            onValueModified: settings.setVehicle_hitchLength = -value
            Connections {
                target: settings
                function onSetVehicle_hitchLengthChanged(){
                    //if the implement type was recently switched from front to rear, the sign of the hitchLength
                    //will be wrong, so make it positive for the GUI
                    if (Number(settings.setVehicle_hitchLength) > 0)
                        hitchLength.setValue(Number(settings.setVehicle_hitchLength))
                    else
                        hitchLength.setValue(-Number(settings.setVehicle_hitchLength))
                }
            }
         }
    }
}
