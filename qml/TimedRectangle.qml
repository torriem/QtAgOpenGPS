import QtQuick 2.0

/* This type defines a popup message box that displays a message for
 * a certain amount of time, then dismisses itself.  Also it can
 * display several messages at one time. Popup can be closed and all
 * messages canceled if the user presses "Esc" or clicks outside the
 * popup.
 *
 * The following methods are defined as javascript functions:
 * cancelAllMessages() - cancel all displayed messages and close popup
 * addMessage(timeout, title, message) - Popup (if not open already)
 *        and display a title and message, and timeout after
 *        timeout milliseconds.
 */
Rectangle {
    id: timerR
    property int timeoutSet: 5000
    property int timeout: 5000
    onVisibleChanged: {
        if(timerR.visible === true){
            timeout = timeoutSet
            timer.running = true
        }
    }

    Timer {
        id: timer
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            timerR.timeout = timerR.timeout - interval
            if(timerR.timeout < 0){
                timer.running = false
                timerR.visible = false
            }
        }
    }
}
