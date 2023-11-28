import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        Image {
            source: "qrc:/images/Config/ConT_TramSpacing.png"
            id: imageTram
            anchors.right: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 10
            width: 200
            height: 200
        }
        SpinBox{
            id: tramWidth
            anchors.verticalCenter: imageTram.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.margins: 3
            editable: true
            Text{
                anchors.bottom: parent.top
                anchors.horizontalCenter:parent.horizontalCenter
                text: qsTr("Tram Width")
            }
            Text{
                anchors.left: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            width: 200
            height: 200
            border.color: "black"
            radius: 5
            IconButtonTransparent{
                anchors.fill: parent
                icon: "qrc:/images/Config/ConT_TramOverride.png"
            }
        }
    }
}
