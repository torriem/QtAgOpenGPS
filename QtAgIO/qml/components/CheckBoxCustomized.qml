import QtQuick
import QtQuick.Controls

/*The default CheckBox has the text with about 70% opacity.
  So it's almost useless to us. If I can't see it on my computer,
  How are we going to see it on a tablet in the tractor? */

CheckBox{
    id: ckbox_root
    text: ""
    contentItem: Text{
        text: ckbox_root.text
        font.pixelSize: 13
        color: "black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
    }
}
