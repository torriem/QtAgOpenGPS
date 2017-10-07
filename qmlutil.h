#ifndef QMLUTIL_H
#define QMLUTIL_H

#include <QQuickItem>

static inline QObject *qmlItem(QQuickItem *root, QString name)
{
    return root->findChild<QObject *>(name);

}

#endif // QMLUTIL_H
