import QtQuick
import QtQuick.Controls

Item{
    id: itemCombo
    property string text: ""
    property alias model: rootCombo.model
    property alias editable: rootCombo.editable
    property alias currentIndex: rootCombo.currentIndex
    height: 75
    width: rootCombo.width
    TextLine{
        anchors.top: parent.top
        text: itemCombo.text
    }
    ComboBox{
        id: rootCombo
        editable: true
        implicitWidth: 200
        implicitHeight: 50
        anchors.bottom: parent.bottom
        model: ListModel{
            id: rootComboModel
        }
        onAccepted: {
            if (rootCombo.find(currentText) === -1){
                rootComboModel.append({text: editText})
                currentIndex = rootCombo.find(editText)
            }
        }
    }
}