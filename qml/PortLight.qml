import QtQuick 2.0

Rectangle {
    id: portLight
    radius: parent.height / 10
    state: "closed"

    State {
        name: "closed"
        PropertyChanges { target: portLight; color: "grey" }
    }
    State {
        name: "error"
        PropertyChanges { target: portLight; color: "red" }
    }
    State {
        name: "ok"
        PropertyChanges { target: portLight; color: "green" }
    }
}
