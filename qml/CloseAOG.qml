import QtQuick
import QtQuick.Dialogs

MessageDialog {
    buttons: MessageDialog.Ok
    modality: Qt.WindowModal
    parentWindow: mainWindow
    text: qsTr("AgOpenGPS is already running")
    onButtonClicked: Qt.quit()
}
