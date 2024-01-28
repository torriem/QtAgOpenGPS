import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: fieldData
    width: 200
    height: 500
    color: "black"
    opacity: 0.7
    border.color: "black"
    border.width: 1.5
    Column{
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        TextLine{
            text: qsTr("Total: "+ aog.totalAcres)
        }
        TextLine{
            text: qsTr("Worked")
        }
        TextLine{
            text: "--------------------"
        }

        TextLine {
            text: qsTr("Applied: "+ aog.workedAcres)
        }
        TextLine{
            text: qsTr("Remain: "+ aog.totalAcres - aog.workedAcres +
                       "<br>" + "%<br>" + "hrs")
        }
        TextLine{
            text: qsTr("Actual<br>--------------------")
        }

        TextLine {
            text: qsTr("Applied: "+ aog.actualAcres)
        }
        TextLine{
            text: qsTr("Remain: " + aog.totalAcres - aog.actualAcres)
        }
        TextLine{
            text: qsTr("Overlap: " + "IDK")
        }
        TextLine {
            text: qsTr("Rate: "+ "ha/hr")
        }
    }
}
