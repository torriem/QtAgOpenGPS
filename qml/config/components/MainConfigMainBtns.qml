import QtQuick

import "../../components"
import "../.."

IconButtonTransparent {
    width: 100 * theme.scaleWidth
    height: 75 * theme.scaleHeight

    checkable: true
    radius: 0

    property bool toggle: false
    onClicked: if (toggle) { checked = false ; toggle = false }
    onPressed: if (checked) toggle=true
}
