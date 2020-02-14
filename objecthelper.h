#ifndef OBJECTHELPER_H
#define OBJECTHELPER_H

#include <QObject>

class ObjectHelper : public QObject
{
    Q_OBJECT
public:
    explicit ObjectHelper(QObject *parent = nullptr);

signals:

public slots:
};

#endif // OBJECTHELPER_H
