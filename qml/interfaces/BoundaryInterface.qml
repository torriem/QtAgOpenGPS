import QtQuick 2.15

Item {
    id: boundaryInterface

    property bool isBoundaryBeingMade: false
    property double createBndOffset: 0
    property bool isDrawRightside: false

    property double area: 0
    property int pts: 0


    //whenever we manually add a point, we need to recalculate area
    //otherwise the main form should do this automatically as we drive
    //should update beingMadeArea and beingMadePts above
    signal calculateArea()
    signal stop(bool save) //save or cancel boundary being made
    signal add_point() //add the current location as a boundary point
    signal delete_last_point()
    signal restart()




    property var boundary_list: [
        { index: 0, area: 75, drive_through: false},
        { index: 1, area: 5, drive_through: false},
        { index: 2, area: 6, drive_through: true}
    ]


}
