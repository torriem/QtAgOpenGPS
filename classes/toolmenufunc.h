#ifndef TOOLMENUFUNC_H
#define TOOLMENUFUNC_H

#include <QtQuick/QQuickItem>
#include <QQuickView>

class toolMenufunc : public QQuickView
{
    Q_OBJECT
public:
    toolMenufunc();
    QQuickItem *qml_root;

    QObject *btndelAppliedArea;

    void miscfunc();
};

#endif // TOOLMENUFUNC_H
