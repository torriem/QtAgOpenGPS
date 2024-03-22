import QtQuick 2.0
import QtQuick.Controls 2.5

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
