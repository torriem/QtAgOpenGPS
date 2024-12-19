// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
#include "qmlsectionbuttons.h"

QMLSectionButtons::QMLSectionButtons(QObject *parent): QObject(parent)
{
}

void QMLSectionButtons::onStatesUpdated(void) {
    //if QML updated things we need to re-read the items
    //formgps_ui connects aog.sectionButtonStateChanged to this function,
    //used in CTool.sectionButtonState
    needRead = true;
}
