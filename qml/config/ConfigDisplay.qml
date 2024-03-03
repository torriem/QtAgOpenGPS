import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ".."
Item {
    anchors.fill: parent
    Rectangle{
        id: configDisplay
        anchors.fill: parent
        color: "ghostwhite"
        GridLayout{
            id:buttonsMain
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.bottom: units.top
            anchors.left: parent.left
            anchors.right: parent.right
            rows: 6
            columns:2
            rowSpacing: 10
            flow: Grid.TopToBottom
            IconButtonColor{
                id: fieldTexture
                objectName: "btnFieldTexture"
                text: qsTr("Field Texture")
                icon.source: "/images/Config/ConD_FloorTexture.png"
                checkable: true
                isChecked: settings.setDisplay_isTextureOn
                onCheckedChanged: settings.setDisplay_isTextureOn = checked
            }
            IconButtonColor{
                id: autoDayNight
                objectName: "btnAutoDayNight"
                isChecked: settings.setDisplay_isAutoDayNight
                onCheckedChanged: settings.setDisplay_isAutoDayNight = !checked
                checkable: true
                text: qsTr("Auto Day Night")
                icon.source: "/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                objectName: "btnStartFullScreen"
                text: qsTr("Start FullScreen")
                icon.source: "/images/Config/ConD_FullScreenBegin.png"
                checkable: true
                isChecked: settings.setDisplay_isStartFullScreen
                onCheckedChanged: settings.setDisplay_isStartFullScreen = checked
            }
            IconButtonColor{
                id:grid
                objectName: "btnGrid"
                text: qsTr("Grid")
                icon.source: "/images/Config/ConD_Grid.png"
                checkable: true
                isChecked: settings.setMenu_isGridOn
                onCheckedChanged: settings.setMenu_isGridOn = checked
            }
            IconButtonColor{
                id:sky
                objectName: "btnSky"
                text:qsTr("Sky")
                icon.source: "/images/Config/ConD_Sky.png"
                checkable: true
                isChecked: settings.setMenu_isSkyOn
                onCheckedChanged: settings.setMenu_isSkyOn = checked
            }
            IconButtonColor{
                id:brightness
                text:qsTr("Brightness")
                objectName: "btnBrightness"
                icon.source: "/images/BrightnessUp.png"
                checkable: true
                isChecked: settings.setDisplay_isBrightnessOn
                onCheckedChanged: settings.setDisplay_isBrightnessOn = checked
            }
            IconButtonColor{
                id:lightBar
                objectName: "btnLightBar"
                text:qsTr("Lightbar")
                icon.source: "/images/Config/ConD_LightBar.png"
                checkable: true
                isChecked: settings.setMenu_isLightbarOn
                onCheckedChanged: settings.setMenu_isLightbarOn = checked
            }
            IconButtonColor{
                id:logNMEA
                objectName: "btnLonNMEA"
                text: qsTr("Log NMEA")
                checkable: true
                icon.source: "/images/Config/ConD_LogNMEA.png"
            }
            /*
            IconButtonColor{ //delete this?
                id:keyboard
                objectName: "btnKeyboard"
                text:qsTr("Keyboard")
                icon.source: "/images/Config/ConD_KeyBoard.png"
                checkable: true
                isChecked: false
            }*/
            IconButtonColor{
                id: guideLines
                objectName: "btnGuidelines"
                text: qsTr("GuideLines")
                icon.source: "/images/Config/ConD_ExtraGuides.png"
                checkable: true
                isChecked: settings.setMenu_isSideGuideLines
                onCheckedChanged: settings.setMenu_isSideGuideLines = checked
            }
            IconButtonColor{
                id:svennArrow
                objectName: "btnSvennArrow"
                text: qsTr("Svenn Arrow")
                icon.source: "/images/SvennArrow.png"
                checkable: true
                isChecked: settings.setDisplay_isSvennArrowOn
                onCheckedChanged: settings.setDisplay_isSvennArrowOn
            }
        }
        Row{
            id:units
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: childrenRect.width
            height: childrenRect.height
            ButtonGroup {
                id: metricimp
                buttons: [metric, imperial]
            }

            IconButtonColor{
                id:metric
                objectName: "btnMetric"
                icon.source: "/images/Config/ConD_Metric.png"
                //text: qsTr("Metric")
                property bool settingsChecked: settings.setMenu_isMetric
                checkable: true
                isChecked: settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = checked
            }
            IconButtonColor{
                id:imperial
                objectName: "btnImperial"
                icon.source: "/images/Config/ConD_Imperial.png"
                text: ""
                checkable: true
                isChecked: !settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = !checked
            }
        }
    }
}
