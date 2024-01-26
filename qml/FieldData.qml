import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: fieldData
    width: 200
    height: 500
    anchors.fill: parent
    color: "black"
    Column{
        Text{
        text: "Total: "+ aog.totalAcres
        }
        Text {
            text: "Worked: "+ aog.workedAcres
        }
        Text {
            text: "Worked: "+ aog.actualAcres
        }
    }

}
