import QtQuick
import QtQuick.Controls

ScrollView{
    id: scrollViewRoot
    property color textColor: "black"
    property color backgroundColor: "white"
    property color borderColor: "black"
    property color borderColorFroze: "blue"
    property bool freezable: true
    property bool froze: false
    property int borderWidth: 2
    property int maxLines: 20
    width: backgroundRect.width

    function append(message) {
        if(froze) return //do nothing

        //append the new mesage
        textAreaRoot.append(message)

        // Check if the number of lines exceeds the limit
        if (textAreaRoot.text.split("\n").length > maxLines) {
            let lines = textAreaRoot.text.split("\n");
            lines = lines.slice(lines.length - maxLines);  // Keep only the last `maxLines` lines
            textAreaRoot.text = lines.join("\n");
        }
        // Scroll to the bottom automatically if not frozen
        flickableContent.flick(0, -10000)
    }
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
        interactive: !scrollViewRoot.froze

    }
    Flickable {
        id: flickableContent
        clip: true
        contentHeight: textAreaRoot.contentHeight
        boundsMovement:Flickable.StopAtBounds

        TextArea{
            id: textAreaRoot
            color: scrollViewRoot.textColor
            readOnly: true
            font.pixelSize: 12
            width: flickableContent.width
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    scrollViewRoot.froze = !scrollViewRoot.froze
                }
            }
        }
    }
    background: Rectangle{
        id: backgroundRect
        property bool froze: scrollViewRoot.froze
        color: scrollViewRoot.backgroundColor
        border.color: scrollViewRoot.borderColor
        border.width: scrollViewRoot.borderWidth
        onFrozeChanged: {
            if (froze)
                border.color = borderColorFroze
            else
                border.color = borderColor
        }
    }
}
