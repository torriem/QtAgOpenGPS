#ifndef AGIOSETTINGS_H
#define AGIOSETTINGS_H

#include <QSettings>
#include <QDebug>
#include <QStandardPaths>

class AgIOSettings : public QObject
{
    Q_OBJECT
public:
    explicit AgIOSettings(QObject *parent = nullptr);
    // Declare the global QSettings object
    QSettings settings;
    void generateDefaultSettings(QSettings& settings);
};


#endif // AGIOSETTINGS_H
