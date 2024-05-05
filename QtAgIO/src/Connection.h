#pragma once
#include <QObject>
#include <QString>

/// @brief Base class for a connection to the outside world
class Connection : public QObject
{
    public:
    Connection() = default;
    Connection(QString n) : name(n) {};
    virtual ~Connection() = default;

    QString getName() {return name;};
protected:
    QString name;
};
