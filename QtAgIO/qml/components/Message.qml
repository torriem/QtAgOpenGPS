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
    onVisibleChanged: console.log("MessageDialog vis changed")
	id: message
	title: "Title"
	text: "Message"
    buttons: MessageDialog.Ok
	onAccepted: message.visible = false

	function addMessage(title: string, message: string) {
		message.title = title
		message.text = message
		message.visible = true
	}
}
