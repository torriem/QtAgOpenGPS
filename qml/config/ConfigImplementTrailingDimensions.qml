import QtQuick
import QtQuick.Controls

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
        source: "/images/ToolHitchPageTrailing.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 15
        height: parent.height*.75
    }
    SpinBoxCM{
        id: toolTrailingHitchLength
        anchors.top: image1.bottom
        anchors.right: parent.right
        anchors.rightMargin: 400
        from: 10
        to:3000
        boundValue: -settings.setTool_toolTrailingHitchLength
        onValueModified: settings.setTool_toolTrailingHitchLength = -value
        TextLine{
            text: qsTr("Units: ")+ utils.cm_unit_abbrev()
            font.bold: true
            anchors.top: parent.bottom
        }
    }
}
