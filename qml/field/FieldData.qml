import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."
import "../components"

Rectangle {
    id: fieldData
    width: 200
    height: childrenRect.height + 30
    color: "black"
    opacity: 0.7
    border.color: "black"
    border.width: 1.5
    Column{
        id: column
        anchors.top: parent.top
        anchors.margins: 15
        anchors.horizontalCenter: parent.horizontalCenter
        TextLine{ color: "white"; text: qsTr("Total: ")+ utils.area_to_unit_string(aog.areaBoundaryOuterLessInner, 2)}
        TextLine{ color: "white"; text: qsTr("Worked")}
        TextLine{ color: "white"; text: "---------------"}
        TextLine{ color: "white"; text: qsTr("Applied: ")+ utils.area_to_unit_string(aog.workedAreaTotal, 2)}
        TextLine{ color: "white"; text: qsTr("Remain: ")+ utils.area_to_unit_string((aog.areaBoundaryOuterLessInner - aog.workedAreaTotal), 2)}
        TextLine{ color: "white"; text: Number(aog.percentLeft).toLocaleString(Qt.locale(), 'f', 0)+"%"}
        TextLine{ color: "white"; text: aog.timeTilFinished}
        TextLine{ color: "white"; text: qsTr("Actual")}
        TextLine{ color: "white"; text: "----------------"}
        TextLine{ color: "white"; text: qsTr("Applied: ")+ utils.area_to_unit_string(aog.actualAreaCovered, 2)}
        TextLine{ color: "white"; text: qsTr("Remain: ") + utils.area_to_unit_string((aog.areaBoundaryOuterLessInner - aog.actualAreaCovered), 2)}
        TextLine{ color: "white"; text: qsTr("Overlap: ") + utils.area_to_unit_string((aog.workedAreaTotal - aog.actualAreaCovered), 2)}
        TextLine{ color: "white"; text: aog.workRate}
    }
}
