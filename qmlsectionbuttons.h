#ifndef QMLSECTIONBUTTONS_H
#define QMLSECTIONBUTTONS_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include "btnenum.h"

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
