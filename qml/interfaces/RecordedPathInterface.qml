import QtQuick 2.0

Item {
    id: recordedPathInterface
    signal recPathFollowStop() //go stop in cs
    signal recPathRecordStop()//record stop in cs
    signal recPathResumeStyle()//resume path in cs
    property int resumeStyleFromCpp: 0
    signal recPathSwapAB()
    signal recPathClear()
    signal recPathLoad()

}
