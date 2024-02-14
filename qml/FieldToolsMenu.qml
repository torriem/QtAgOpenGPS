import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Drawer {
    id: fieldToolsMenu

    width: 300
    visible: false
    modal: true

    contentItem: Rectangle {
        id: fieldToolsMenuRect
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.left: parent.left

        color: "black"

        ScrollView {
            anchors.fill: parent
            clip: true

            ColumnLayout {
                anchors.fill: parent
                IconButtonTextBeside{
                    text: "Boundary"
                    icon.source: "/images/MakeBoundary.png"
                    width: 300
                    onClicked: {
                        fieldToolsMenu.visible = false
                        boundaryMenu.show()
                    }
                }
                IconButtonTextBeside{
                    text: "Headland"
                    icon.source: "/images/HeadlandMenu.png"
                    width: 300
                    onClicked: {
                        fieldToolsMenu.visible = false
                        headlandDesigner.show()
                    }
                }
                IconButtonTextBeside{
                    text: "Headland (Build)"
                    icon.source: "/images/Headache.png"
                    width: 300
                    onClicked: {
                        fieldToolsMenu.visible = false
                        headAcheDesigner.show()
                    }
                }
                IconButtonTextBeside{
                    text: "Tram Lines"
                    icon.source: "/images/TramLines.png"
                    width: 300
                    onClicked: tramLinesEditor.visible = true
                }
                IconButtonTextBeside{
                    text: "Recorded Path"
                    icon.source: "/images/RecPath.png"
                    width: 300
                    onClicked: recordButtons.visible = !recordButtons.visible
                }
            }
        }
    }
}
