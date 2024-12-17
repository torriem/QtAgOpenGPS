#ifndef QMLSECTIONBUTTONS_H
#define QMLSECTIONBUTTONS_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include "btnenum.h"

//This object provides a convenient way to access section button states
//stored in AOGInterface.sectionButtonState.  This array is not the same
//thing as the on-screen buttons.  Since there are only 16 on-screen or
//physical buttons allowed, when using zones, one on-screen button
//represents a zone of multiple sections (typically row units), each of
//which has a button state that is either off, auto, or on.

class QMLSectionButtons : public QObject
{
    Q_OBJECT
protected:
    QVariantList states;
    bool needRead = true;

public:
    QMLSectionButtons(QObject * parent = 0);

    static inline QObject *aog_root = NULL;
    static inline void set_aog_root(QObject *aog_root_) { aog_root = aog_root_;}

    inline btnStates get (int sectionno) {
        if (needRead) {
            needRead = false;
            states = aog_root->property("sectionButtonState").toList();
        }

        return (btnStates)states[sectionno].toInt();
    }

    inline void set (int sectionno, btnStates new_state) {
        if (needRead) {
            needRead = false;
            states = aog_root->property("sectionButtonState").toList();
        }

        states[sectionno] = QVariant((int) new_state);
        aog_root->setProperty("sectionButtonState", states);
    }

public slots:
    void onStatesUpdated();

};

#endif // QMLSECTIONBUTTONS_H
