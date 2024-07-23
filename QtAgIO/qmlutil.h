#ifndef QMLUTIL_H
#define QMLUTIL_H

#include <QQuickItem>

static inline QObject *qmlItem(QObject *root, QString name)
{
    return root->findChild<QObject *>(name);

}

#endif // QMLUTIL_H
