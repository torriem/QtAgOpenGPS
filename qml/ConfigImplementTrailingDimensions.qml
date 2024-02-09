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
        source: "/images/ToolHitchPageTrailing.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 15
        height: parent.height*.75
        SpinBoxCM{
            id: toolTrailingHitchLength
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 500
            from: 10
            to:3000
            boundValue: -settings.setTool_toolTrailingHitchLength
            onValueModified: settings.setTool_toolTrailingHitchLength = -value
        }
        SpinBoxCM{
            id: hitchLength
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 50
            from: 10
            to:3000
            boundValue: settings.setVehicle_hitchLength < 0 ? -settings.setVehicle_hitchLength : settings.setVehicle_hitchLength
            onValueModified: settings.setVehicle_hitchLength = -value
        }
    }
}
