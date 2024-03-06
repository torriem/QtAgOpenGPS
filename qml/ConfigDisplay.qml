import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

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
                text: qsTr("Field Texture")
                icon.source: "/images/Config/ConD_FloorTexture.png"
                checkable: true
                isChecked: settings.setDisplay_isTextureOn
                onCheckedChanged: settings.setDisplay_isTextureOn = checked
            }
            IconButtonColor{
                id: autoDayNight
                isChecked: settings.setDisplay_isAutoDayNight
                onCheckedChanged: settings.setDisplay_isAutoDayNight = !checked
                checkable: true
                text: qsTr("Auto Day Night")
                icon.source: "/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                text: qsTr("Start FullScreen")
                icon.source: "/images/Config/ConD_FullScreenBegin.png"
                checkable: true
                isChecked: settings.setDisplay_isStartFullScreen
                onCheckedChanged: settings.setDisplay_isStartFullScreen = checked
            }
            IconButtonColor{
                id:grid
                text: qsTr("Grid")
                icon.source: "/images/Config/ConD_Grid.png"
                checkable: true
                isChecked: settings.setMenu_isGridOn
                onCheckedChanged: settings.setMenu_isGridOn = checked
            }
            IconButtonColor{
                id:sky
                text:qsTr("Sky")
                icon.source: "/images/Config/ConD_Sky.png"
                checkable: true
                isChecked: settings.setMenu_isSkyOn
                onCheckedChanged: settings.setMenu_isSkyOn = checked
            }
            IconButtonColor{
                id:brightness
                text:qsTr("Brightness")
                icon.source: "/images/BrightnessUp.png"
                checkable: true
                isChecked: settings.setDisplay_isBrightnessOn
                onCheckedChanged: settings.setDisplay_isBrightnessOn = checked
            }
            IconButtonColor{
                id:lightBar
                text:qsTr("Lightbar")
                icon.source: "/images/Config/ConD_LightBar.png"
                checkable: true
                isChecked: settings.setMenu_isLightbarOn
                onCheckedChanged: settings.setMenu_isLightbarOn = checked
            }
            IconButtonColor{
                id:logNMEA
                text: qsTr("Log NMEA")
                checkable: true
                icon.source: "/images/Config/ConD_LogNMEA.png"
            }
            IconButtonColor{
                id: guideLines
                text: qsTr("GuideLines")
                icon.source: "/images/Config/ConD_ExtraGuides.png"
                checkable: true
                isChecked: settings.setMenu_isSideGuideLines
                onCheckedChanged: settings.setMenu_isSideGuideLines = checked
            }
            IconButtonColor{
                id:svennArrow
                text: qsTr("Svenn Arrow")
                icon.source: "/images/SvennArrow.png"
                checkable: true
                isChecked: settings.setDisplay_isSvennArrowOn
                onCheckedChanged: settings.setDisplay_isSvennArrowOn
            }
            IconButtonColor{
                id: compass
                text: qsTr("Compass")
                icon.source: "/images/Images/z_Compass.png"
                checkable: true
                isChecked: settings.setMenu_isCompassOn
                onCheckedChanged: settings.setMenu_isCompassOn
            }
            /*IconButtonColor{ //appears there is no setting for this
                id: speedo
                text: qsTr("Speedometer")
                icon.source: "/images/Images/z_Speedo.png"
                checkable: true
                isChecked: settings.setMenu_isSpeedoOn
                onCheckedChanged: settings.setMenu_isSpeedoOn
            }*/
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
                icon.source: "/images/Config/ConD_Metric.png"
                //text: qsTr("Metric")
                property bool settingsChecked: settings.setMenu_isMetric
                checkable: true
                isChecked: settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = checked
            }
            IconButtonColor{
                id:imperial
                icon.source: "/images/Config/ConD_Imperial.png"
                text: ""
                checkable: true
                isChecked: !settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = !checked
            }
        }
    }
}
