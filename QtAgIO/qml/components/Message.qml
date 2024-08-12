import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

/*This type defines a message box that pops up until the user clicks
 * it. For example, when loopback error, like port already taken.
 * We can expand on it eventually, but for now, it will have an "OK"
 * button.
 */

MessageDialog {
	visible: false
    id: messageDia
	title: "Title"
	text: "Message"
    buttons: MessageDialog.Ok
    onAccepted: messageDia.visible = false

	function addMessage(title: string, message: string) {
        messageDia.title = title
        messageDia.text = message
        messageDia.visible = true
	}
}
