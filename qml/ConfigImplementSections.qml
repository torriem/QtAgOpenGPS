import QtQuick 2.0
import QtQuick.Controls 2.5


Rectangle{
    id: configImplementSection
    anchors.fill: parent
    color: "ghostwhite"
    visible: false

    Row{
        id: bottomRow
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: bottomRightText.left
        anchors.margins: 10
        spacing: 90
        Button{
            function toggleZones(){
                if(configImplementSectionsSection.visible == true){
                    image.source = "/images/Config/ConT_Symmetric.png"
                    configImplementSectionsZones.visible = true
                    configImplementSectionsSection.visible = false
                    console.log("zones")
                }else{
                    image.source ="/images/Config/ConT_Asymmetric.png"
                    configImplementSectionsZones.visible = false
                    configImplementSectionsSection.visible = true
                    console.log("sections")
                }
            }
            width: 180
            height: 130
            id: chooseZones
            objectName: "zonesOrSections"
            onClicked: {
                toggleZones()
            }
            background: Rectangle{
                color: "white"
                border.color: "black"
                border.width: 1
                Image{
                    id: image

                    source:"/images/Config/ConT_Asymmetric.png"
                    anchors.fill: parent
                }
            }
        }
        SpinBoxCustomized{
            objectName: "percentConverage"
            fromVal: 0
            toVal: 100
            valueVal: 100
            anchors.bottom: parent.bottom
            title: "% Coverage"
//            Text{
//                anchors.bottom: parent.top
//                text: "% Coverage"
//            }
        }
        IconButton{
            objectName: "boundaryOff"
            icon.source: "/images/SectionOffBoundary.png"
            iconChecked: "/images/SectionOnBounary.png"
            anchors.bottom: parent.bottom
            implicitWidth: 100
            implicitHeight: 100
            border: 1
            color3: "white"
            colorChecked1: "green"
            colorChecked2: "green"
            colorChecked3: "green"
        }
        SpinBox{
            objectName: "speedBelowSectionOff"
            from: 0
            to: 30
            value: 0
            editable: true
            anchors.bottom: parent.bottom
            Image{
                anchors.bottom: parent.top
                width: parent.width
                height: parent.height +30
                source: "/images/SectionOffBelow.png"
            }
        }
    }
    Text{
        id: bottomRightText
        anchors.right: parent.right
        text: "Km/H"
        anchors.verticalCenter: bottomRow.verticalCenter
        font.pixelSize: 25
    }
    ConfigImplementSectionsSection{
        id: configImplementSectionsSection
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: 30
        anchors.margins: 15
        visible: true
    }
    ConfigImplementSectionsZones{
        id: configImplementSectionsZones
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: 30
        anchors.margins: 15
        visible: false
    }
}
