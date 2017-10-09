#ifndef AOGSETTINGS_H
#define AOGSETTINGS_H

#include <QSettings>
#include <QVariant>

class AOGSettings : public QSettings
{
public:
    QVariant value(const QString &key, const QVariant &defaultvalue);
};

#endif // AOGSETTINGS_H
