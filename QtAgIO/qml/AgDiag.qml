import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    /*Dialog {
        property bool dontShow: false
        visible: false
        Connections{
            target: agio
            function onNmeaErrorChanged(){
                if(!dianmeaError.dontShow) dianmeaError.visible = true
            }
        }

        id: dianmeaError
        title: qsTr("NMEA Error!")
        text: qsTr("Your elevation changed 1 M between GGA Sentences
Either you are recieving NMEA from two sources, or you just rolled your vehicle.")
             {
            visible: true
            id: dontShowAgainCheckbox
            text: "Don't show again"
            onCheckedChanged: dianmeaError.dontShow = true;
            anchors.left: dianmeaError.left
            anchors.leftMargin: 10
            anchors.bottom: dianmeaError.bottom
        }
            Button{
                anchors.right: dianmeaError.right
                anchors.bottom: dianmeaError.bottom
                anchors.rightMargin: dianmeaError.right
                onClicked: dianmeaError.close()
            }
    }*/
}
