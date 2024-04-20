import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"
Item{
    id: settingsWindow
    visible: settingsBtn.checked
    anchors.centerIn: parent
    SpinBoxDecimal{
        text: "Panic Stop"
        implicitWidth: 100
        implicitHeight: 65
        from: 0
        value: 0
        to: 10
        editable: true
    }
}
