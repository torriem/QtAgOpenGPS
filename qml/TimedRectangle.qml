import QtQuick 2.0

/* More or less a modified version of TimedMessage
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
