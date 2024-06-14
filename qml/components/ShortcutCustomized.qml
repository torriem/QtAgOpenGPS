import QtQuick

Shortcut{
	id: shortcut
	property int hotkeys: 0
	property var hotkeysVar: settings.setKey_hotkeys
	sequences: [hotkeysVar[hotkeys], ("Shift+"+ hotkeysVar[hotkeys])]
}
