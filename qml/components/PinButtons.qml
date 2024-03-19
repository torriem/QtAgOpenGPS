import QtQuick 2.9
import QtQuick.Controls 2.5
ComboBox {
    property string pinNumber: ""
    id: pinButtons
    editable: false
    model: ListModel {
        id: a2Dmodel
        ListElement {text: "-"}
        ListElement {text: "Section 1"}
        ListElement {text: "Section 2"}
        ListElement {text: "Section 3"}
        ListElement {text: "Section 4"}
        ListElement {text: "Section 5"}
        ListElement {text: "Section 6"}
        ListElement {text: "Section 7"}
        ListElement {text: "Section 8"}
        ListElement {text: "Section 9"}
        ListElement {text: "Section 10"}
        ListElement {text: "Section 11"}
        ListElement {text: "Section 12"}
        ListElement {text: "Section 13"}
        ListElement {text: "Section 14"}
        ListElement {text: "Section 15"}
        ListElement {text: "Section 16"}
        ListElement {text: "Hyd Up"}
        ListElement {text: "Hyd down"}
        ListElement {text: "Tram Right"}
        ListElement {text: "Tram Left"}
        ListElement {text: "Geo Stop"}
    }
    /*onAccepted: {
        if (skips.find(currentText) === -1){
            pinButtons.append({text: editText})
            curentIndex = a2Dconverter.find(editText)
        }
    }*/
    implicitHeight:30
    implicitWidth: 150
    Text{
        anchors.bottom: parent.top
        anchors.left: parent.left
        text: pinButtons.pinNumber

    }
}
