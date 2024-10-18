import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

/*This type defines a message box that pops up until the user clicks
 * it. For example, when loopback error, like port already taken.
 * We can expand on it eventually, but for now, it will have an "OK"
 * button.
 */

MessageDialog {
    property bool doWeQuit: false
	visible: false
    onVisibleChanged: if (visible) {
                          mainWindow.show()
                          mainWindow.raise()
                      }
    id: messageDia
	title: "Title"
	text: "Message"
    buttons: MessageDialog.Ok

    onAccepted: {
        if(doWeQuit)
            Qt.quit()

        messageDia.visible = false
        mainWindow.lower()
    }

    function addMessage(title: string, message: string, doWeQuit: bool) {
        messageDia.title = title
        messageDia.text = message
        messageDia.doWeQuit = doWeQuit
        messageDia.visible = true
	}
}
