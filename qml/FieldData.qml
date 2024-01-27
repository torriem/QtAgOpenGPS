import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: fieldData
    width: 200
    height: 500
    color: "transparent"
    border.color: "black"
    border.width: 1.5
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
