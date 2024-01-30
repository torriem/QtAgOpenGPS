import QtQuick 2.0
import QtQuick.Controls 2.5

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
        TextLine{ text: qsTr("Total: "+ utils.area_to_unit_string(aog.areaBoundaryOuterLessInner, 2))}
        TextLine{ text: qsTr("Worked")}
        TextLine{ text: "---------------"}
        TextLine{ text: qsTr("Applied: "+ utils.area_to_unit_string(aog.workedAreaTotal, 2))}
        TextLine{ text: qsTr("Remain: "+ utils.area_to_unit_string((aog.areaBoundaryOuterLessInner - aog.workedAreaTotal), 2))}
        TextLine{ text: qsTr("%")}
        TextLine{ text: qsTr("hrs")}
        TextLine{ text: qsTr("Actual")}
        TextLine{ text: "----------------"}
        TextLine{ text: qsTr("Applied: "+ utils.area_to_unit_string(aog.actualAreaCovered, 2))}
        TextLine{ text: qsTr("Remain: " + utils.area_to_unit_string((aog.areaBoundaryOuterLessInner - aog.actualAreaCovered), 2))}
        TextLine{ text: qsTr("Overlap: " + utils.area_to_unit_string((aog.workedAreaTotal - aog.actualAreaCovered), 2))}
        TextLine{ text: qsTr("Rate: "+ utils.areaRate_unit())}
    }
}
