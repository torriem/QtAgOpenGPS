import QtQuick
import QtQuick.Dialogs

MessageDialog {
    buttons: MessageDialog.Ok
    text: qsTr("AgIO is already running")
    onButtonClicked: Qt.quit()
}
