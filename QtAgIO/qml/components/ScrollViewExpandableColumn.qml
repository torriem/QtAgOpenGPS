import QtQuick
import QtQuick.Controls.Fusion

ScrollView{
    id: rootScrollView
    default property alias content: expandableColumn.children
    onHeightChanged: expandableColumn.adjust(height)
    onVisibleChanged: expandableColumn.adjust(height)
    clip: true
    width: expandableColumn.width
    ExpandableColumn{
        id: expandableColumn
        width: children.width
    }
}
