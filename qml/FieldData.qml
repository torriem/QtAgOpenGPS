import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: fieldData
    width: 200
    height: childrenRect.height + 30
    color: "#4d4d4d"
    Column{
        id: column
        anchors.top: parent.top
        anchors.margins: 15
        anchors.horizontalCenter: parent.horizontalCenter
        TextLine{ text: qsTr("Total: "+ aog.totalAcres)}
        TextLine{ text: qsTr("Worked")}
        TextLine{ text: "---------------"}
        TextLine{ text: qsTr("Applied: "+ aog.worked)}
        TextLine{ text: qsTr("Remain: "+ (aog.totalAcres - aog.workedAcres))}
        TextLine{ text: qsTr("%")}
        TextLine{ text: qsTr("hrs")}
        TextLine{ text: qsTr("Actual")}
        TextLine{ text: "----------------"}
        TextLine{ text: qsTr("Applied: "+ aog.actual)}
        TextLine{ text: qsTr("Remain: " + aog.totalAcres - aog.actual)}
        TextLine{ text: qsTr("Overlap: " + "IDK")}
        TextLine{ text: qsTr("Rate: "+ "ha/hr")}
    }
}
