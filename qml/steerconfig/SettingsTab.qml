import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."
import "../components"
Item{
    id: settingsWindow
    visible: settingsBtn.checked
    anchors.centerIn: parent
    SpinBoxOneDecimal{
        text: "Panic Stop"
        implicitWidth: 100
        implicitHeight: 65
        from: 0
        value: 0
        to: 10
        editable: true
    }
}
