// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Displayed on startup. Disclaimer
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import "components" as Comp

Rectangle{
    id: startUp
    anchors.fill: parent
    color: "#ccccff"
	focus: true
	Keys.onEnterPressed: {
		ok.clicked()
	}
	Keys.onReturnPressed: {
		ok.clicked()
	}
    MouseArea{
        anchors.fill: parent
    }

    Comp.TextLine{
        id: about
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
        text: qsTr("About AgOpenGPS")
        font.pixelSize: 10
    }
    AnimatedImage {
        id: gif
        source: prefix + "/images/First.gif"
        anchors.top: about.bottom
        anchors.left: parent.left
        cache: true
        anchors.margins: 5
        width: parent.width/2
        height: parent.height/2
    }
    Column{
        id: disclaimer
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: gif.right
        anchors.margins: 30
        spacing: 10
        Text{
            text: qsTr("Version 0.0")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("Terms and Conditions")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("If you distribute copies of this FREE program, whether gratis or for a fee, impro" +
                       "ved, changed, you must pass on to the recipients the same freedoms that you rece" +
                       "ived as Outlined in the GPLv3 License.")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("You must make sure that they, too, receive or can get the source code. And you must"+
                       " show them these terms so they know their rights. Don't steal others work and call it your own")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("AgOpenGPS is strictly educational software to learn about guidance systems, GPS sentences"+
                       ", and theoretical application of machine control.")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text {
            text: qsTr("It is in no way to be used on real equipment in any circumstance. A simulator is provided"+
                       " to allow the student to safely experience machine guidance.")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("There are not safety systems in the software and none are implied.")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }
        Text{
            text: qsTr("Use at your own Risk")
            wrapMode: Text.WordWrap
            width: disclaimer.width
        }

    }

    Column{
        id: links
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: childrenRect.width
        height: childrenRect.height
        anchors.margins: 10
        spacing: 10
//        Text{
//            text: qsTr("Discussions at...")
//        }
//        Text{
//            text: "<a href='https://discourse.agopengps.com'>https://discourse.agopengps.com</a>"
//            textFormat: Text.RichText
//        }
        Text{
            text: qsTr("Official Repository")
        }
        Text{
            text: "<a href='https://github.com/torriem/QtAgOpenGPS'>https://github.com/torriem/QtAgOpenGPS</a>"
        }
    }
    RowLayout{
        id: buttons
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 50
        anchors.bottomMargin: 40
        height: 100
        width: parent.width/2
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/Cancel64.png"
            onClicked: Qt.quit()
            Text{
                text: qsTr("Disagree")
                anchors.top: parent.bottom
            }
        }
        Comp.IconButtonTransparent{
			id: ok
            icon.source: prefix + "/images/OK64.png"
            onClicked: startUp.visible = false
            Text{
                text: qsTr("I Agree to the Terms<br> and Conditions")
                anchors.top: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
