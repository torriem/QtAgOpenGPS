import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
/* todo:
  not sure about uTurn Style. setting seems to be an int.
  */
Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        GridLayout{
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            flow:Grid.LeftToRight
            rows: 3
            columns: 4
            height: parent.height /2

            Image {
                source: "/images/Config/ConU_UturnRadius.png"
                height: 150
                width: 150
            }
            Image {
                source: "/images/Config/ConU_UturnDistance.png"
                height: 150
                width: 150
            }
            Image {
                source: "/images/Config/ConU_UturnLength.png"
                height: 150
                width: 150
            }
            Image {
                source: "/images/Config/ConU_UturnSmooth.png"
                height: 150
                width: 150
            }
            SpinBoxTwoDecimal{
                objectName: "UturnRadius"
                from: 2
                value: settings.set_youTurnRadius
                onValueChanged: settings.set_youTurnRadius = value
                to:100
                editable: true
            }
            SpinBoxTwoDecimal{
                objectName: "UturnDistance"
                from: 0
                value: settings.set_youTurnDistanceFromBoundary
                onValueChanged: settings.set_youTurnDistanceFromBoundary = value
                to:100
                editable: true
            }
            SpinBoxCustomized{
                objectName: "UturnExtensionLength"
                from: 3
                value: settings.set_youTurnExtensionLength
                onValueChanged: settings.set_youTurnExtensionLength = value
                to:50
                editable: true
            }
            SpinBoxCustomized{
                objectName: "UturnSmoothing"
                from: 8
                value: settings.setAS_uTurnSmoothing
                onValueChanged: settings.setAS_uTurnSmoothing = value
                to:50
                editable: true
            }
            Text{
                text: qsTr("ft")
                font.bold: true
            }
            Text{
                text: qsTr("ft")
                font.bold: true
            }
            Text{
                text: qsTr("ft
Set Extension
Length to 2 or
3x Radius")
                font.bold: true
            }
            Text{
                text: qsTr("ft
Set Smoothing
 to 3 or 4x
Radius")
                font.bold: true
            }
        }
        Rectangle{
            id: uturnStyle
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 150
            height: 150
            Text{
                id: styleText
                text: qsTr("UTurn Style")
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: styleText.bottom
                width:childrenRect.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.topMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                spacing: 20
                IconButtonColor{
                    icon.source: "/images/YouTurnNo.png"
                    //onClick: childUturn.color = "green"
                    /* background:Rectangle{
                    id: childUturn
                    color: "white"
                    border.color: black
                }*/
                }
                IconButtonColor{
                    icon.source: "/images/YouTurnH.png"
                }
            }
        }
    }
}
