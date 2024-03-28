import QtQuick

Item {
    id: recordedPathInterface

    property bool isDrivingRecordedPath: false

    property var recordedPathList: [
        { index: 0, name: "test1" },
        { index: 1, name: "test2" }
    ]

    signal updateLines()
    signal open_path(string name)
    signal delete_path(string name)
    signal start_driving()
    signal stop_driving()
    signal clear_path()

}
