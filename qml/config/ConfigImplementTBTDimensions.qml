import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

/*todo:
//in the application output we get the warning from this page:
qrc:/qml/ConfigImplementDimensions.qml:26:9: QML SpinBoxCM: Binding loop detected for property "value"
qrc:/qml/ConfigImplementDimensions.qml:16:9: QML SpinBoxCM: Binding loop detected for property "value"
I'm not sure why, I think I did them the same as everywhere else.
*/
Rectangle{
    id: configImpDimWin
    anchors.fill: parent
    color: aog.backgroundColor
    visible: false
    Image{
        id: image1
        source: prefix + "/images/ToolHitchPageTBT.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
		anchors.topMargin: 7 * theme.scaleHeight
		anchors.bottomMargin: 7 * theme.scaleHeight
		anchors.leftMargin: 7 * theme.scaleWidth
		anchors.rightMargin: 7 * theme.scaleWidth
        height: parent.height*.75
        SpinBoxCM{
            id: toolTrailingHitchLength
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.rightMargin: 80 * theme.scaleWidth
            from: 10
            to:3000
            boundValue: -settings.setTool_toolTrailingHitchLength
            onValueModified: settings.setTool_toolTrailingHitchLength = -value
        }
        SpinBoxCM{
            id: toolTBTHitchLength
            anchors.top: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 130 * theme.scaleWidth
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
            anchors.topMargin: toolTrailingHitchLength.height + (15 * theme.scaleHeight)
        }
}
