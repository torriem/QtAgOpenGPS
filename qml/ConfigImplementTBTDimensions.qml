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
        id: image1
        source: "/images/ToolHitchPageTBT.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 15
        height: parent.height*.75
        SpinBoxCM{
            id: toolTrailingHitchLength
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.rightMargin: 80
            from: 10
            to:3000
            boundValue: -settings.setTool_toolTrailingHitchLength
            onValueModified: settings.setTool_toolTrailingHitchLength = -value
        }
        SpinBoxCM{
            id: toolTBTHitchLength
            anchors.top: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 130
            from: 10
            to:3000
            boundValue: -settings.setVehicle_tankTrailingHitchLength
            onValueModified: settings.setVehicle_tankTrailingHitchLength = -value
        }
    }
        TextLine{
            text: qsTr("Units: ")+ utils.cm_unit_abbrev()
            font.bold: true
            anchors.top: image1.bottom
            anchors.topMargin: toolTrailingHitchLength.height + 15
        }
}
