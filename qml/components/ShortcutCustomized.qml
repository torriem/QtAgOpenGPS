// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// An attemt to make shortcut keys easier. Not used right now
import QtQuick

Shortcut{
	id: shortcut
	property int hotkeys: 0
	property var hotkeysVar: settings.setKey_hotkeys
	sequences: [hotkeysVar[hotkeys], ("Shift+"+ hotkeysVar[hotkeys])]
}
