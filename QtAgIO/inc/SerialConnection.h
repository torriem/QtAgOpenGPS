#pragma once

#include <QSerialPort>
#include "Connection.h"

/// @brief Implements a Serial connection to the outside world
class SerialConnection : public Connection
{
    public
    SerialConnection() = delete;
    SerialConnection(QString n) : name(n) {};

    private
    QSterialPort serialPort
};
