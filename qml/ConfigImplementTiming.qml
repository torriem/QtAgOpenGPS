import QtQuick 2.0
import QtQuick.Controls 2.5

    Rectangle{
        id: configImpTimWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image{
            id: configImpImage
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 70
            anchors.rightMargin: 30
            anchors.leftMargin: 200
            anchors.topMargin: 100
            source: "/images/ImplementSettings.png"

            SpinBox{
                anchors.right: parent.left
                anchors.top: parent.top
                from: 0
                value: 1
                to: 22
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("On (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                from: 0
                value:0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Off (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.bottom: parent.bottom
                from: 0
                value: 0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Delay (secs)")
                }
            }
            SpinBox{
                anchors.left: parent.left
                anchors.leftMargin: 500
                anchors.top: parent.bottom
                anchors.bottomMargin: -10
                from: -984
                value: 0
                to: 984
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Offset")
                }
            }
            SpinBox{
                anchors.right: parent.right
                anchors.rightMargin: 120
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 100
                from: -1181
                value: 0
                to: 1181
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Overlap / Gap")
                }
            }
        }
    }
