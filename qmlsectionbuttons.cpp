#include "qmlsectionbuttons.h"

QMLSectionButtons::QMLSectionButtons(QObject *parent): QObject(parent)
{
}

void QMLSectionButtons::onStatesUpdated(void) {
    //if QML updated things we need to re-read the items
    needRead = true;
}
