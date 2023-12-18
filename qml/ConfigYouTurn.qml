import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
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
                source: "qrc:images/Config/ConU_UturnRadius.png"
                height: 150
                width: 150
            }
            Image {
                source: "qrc:images/Config/ConU_UturnDistance.png"
                height: 150
                width: 150
            }
            Image {
                source: "qrc:images/Config/ConU_UturnLength.png"
                height: 150
                width: 150
            }
            Image {
                source: "qrc:images/Config/ConU_UturnSmooth.png"
                height: 150
                width: 150
            }
            SpinBox{
                objectName: "UturnRadius"
                from: 2
                value: 20
                to:100
                editable: true
            }
            SpinBox{
                objectName: "UturnDistance"
                from: 0
                value: 10
                to:328
                editable: true
            }
            SpinBox{
                objectName: "UturnRadius"
                from: 9
                value: 20
                to:164
                editable: true
            }
            SpinBox{
                objectName: "UturnRadius"
                from: 8
                value: 18
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
                anchors.topMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                spacing: 20
                IconButtonTransparent{
                icon.source: "qrc:/images/YouTurnNo.png"
                //onClick: childUturn.color = "green"
               /* background:Rectangle{
                    id: childUturn
                    color: "white"
                    border.color: black
                }*/
            }
            IconButtonTransparent{
                icon.source: "qrc:/images/YouTurnH.png"
            }
            }
        }
    }
}
