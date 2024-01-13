import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        Image {
            source: "/images/Config/ConT_TramSpacing.png"
            id: imageTram
            anchors.right: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 10
            width: 200
            height: 200
        }
        SpinBoxCustomized{
            id: tramWidth
            anchors.verticalCenter: imageTram.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.margins: 3
            editable: true
            text: qsTr("Tram Width")
            Text{
                anchors.left: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        IconButtonColor{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            width: 200
            height: 200
            icon.source: "/images/Config/ConT_TramOverride.png"
        }
    }
}
