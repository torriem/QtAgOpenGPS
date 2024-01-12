import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.15


Window {
    width: 800
    height: 600

    TimedMessage {
        id: timedMessageBox
    }

    Component.onCompleted: {
	    console.warn("Okay we're ready to display messages.")
        //timedMessageBox.open()
	    timedMessageBox.addMessage(6000,"Long message","This message will last for 6 seconds")
        timedMessageBox.addMessage(4000,"Another message", "This message will last for 4 seconds")
        timedMessageBox.addMessage(2000,"Shorter message", "This will last for only 2 seconds.")
    }
}
