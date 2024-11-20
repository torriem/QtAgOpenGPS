// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
//import QtQuick.Controls.Styles 1.4

/* This type defines a popup message box that displays a message for
 * a certain amount of time, then dismisses itself.  Also it can
 * display several messages at one time. Popup can be closed and all
 * messages canceled if the user presses "Esc" or clicks outside the
 * popup.
 *
 * The following methods are defined as javascript functions:
 * cancelAllMessages() - cancel all displayed messages and close popup
 * addMessage(timeout, title, message) - Popup (if not open already) 
 *        and display a title and message, and timeout after 
 *        timeout milliseconds.
 */ 
Popup {
    padding: 5
    x: parent.width / 2
    y: parent.height * 0.25
    modal: false

    property int fontsize: 14

    id: timedMessage

    Rectangle {
        id: timedMessageR

        width: 500
        height: 200
        anchors.centerIn:parent

        border.color: "red"
        border.width: 4
        color: "#FFFDF5E6" //old lace

        Component {
            id: timedMessageDelegate
            Rectangle {
                id: our_item
                width: 460
                height: textTitle.implicitHeight + textMessage.implicitHeight + 10
                //border.width: 1
                radius: 5
                color: "transparent"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    Item {
                        Text {
                            id: textTitle
                            width: 450
                            font.bold: true
                            font.pointSize: timedMessage.fontsize + 2
                            text: "(" + Number(model.timeout / 1000).toLocaleString(Qt.locale(),'f',0) + ") " + model.title
                        }
                        Text {
                            id: textMessage
                            width: 450
                            font.pointSize: timedMessage.fontsize
                            anchors.top: textTitle.bottom
                            wrapMode: Text.WordWrap

                            text: model.message
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: function() {
                        our_item.color = "light grey"
                        //console.debug("Clicked on TimedMessageModel item" + index)
                    }
                    onCanceled: function() {
                        our_item.color = "transparent"
                    }
                    onReleased: function() {
                        timedMessageModel.remove(index)
                        if (timedMessageModel.rowCount() < 1) {
                            close()
                            //timedMessage.visible = false
                        }
                    }
                }
            }
        }

        ListView {
            id: timedMessageListView
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            ScrollBar.vertical: ScrollBar { }
            clip: true

            model:  ListModel{
                id: timedMessageModel
                /*
                ListElement { timeout: 6000; title: "Hi"; message: "Here's a message with some very long lines indeed. that go way off the edge of the window" }
                ListElement { timeout: 5000; title: "Hi"; message: "Message2" }
                ListElement { timeout: 4000; title: "Hi"; message: "Message3" }
                ListElement { timeout: 3000; title: "Hi"; message: "Message4" }
                ListElement { timeout: 2000; title: "Hi"; message: "Message5" }
                ListElement { timeout: 3000; title: "Hi"; message: "Message6" }
                ListElement { timeout: 4000; title: "Hi"; message: "Message7" }
                ListElement { timeout: 5000; title: "Hi"; message: "Message8" }
                ListElement { timeout: 6000; title: "Hi"; message: "Message9" }
                */
            }

            focus: true

            delegate: timedMessageDelegate

            Component.onCompleted: function() {
                timedMessageListView.positionViewAtEnd()
            }
        }

        Timer {
            interval: 100
            running: true
            repeat: true

            onTriggered: function() {
                for (var i=0; i < timedMessageModel.rowCount(); i++) {
                    timedMessageModel.get(i).timeout -= 100;
                    if (timedMessageModel.get(i).timeout < 0) {
                        //console.warn("Removed item " + i)
                        timedMessageModel.remove(i)
                        i-- //our iteration is now broken
                    }
                }
                
                if (timedMessageModel.rowCount() === 0) {
                    close()
                    //timedMessage.visible = false
                }

                if (timedMessageModel.rowCount() > 3) {
                    timedMessageR.height = 240;
                } else {
                    timedMessageR.height = timedMessageModel.rowCount() * 80
                }
            }
        }
    }

    onClosed: cancelAllMessages()
        
    function cancelAllMessages() {
    	timedMessageModel.clear();
        //console.debug("canceling all messages.")
    }

    function addMessage(timeout: int, title: string, message: string) {
        //console.debug(timeout + " " + title + " " + message)
        timedMessage.open()
            
    	timedMessageModel.append( { timeout: timeout, title: title, message: message });

        if (timedMessageModel.rowCount() > 3) {
            timedMessageR.height = 240;
        } else {
            timedMessageR.height = timedMessageModel.rowCount() * 80
        }

    }
}

