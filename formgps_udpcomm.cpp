#include <QNetworkDatagram>
#include "formgps.h"
#include "aogproperty.h"
#include "cnmea.h"
#include <QHostAddress>

#define BitConverter_ToDouble(data,position) (*(reinterpret_cast<double *>(&(data[position]))))
#define BitConverter_ToSingle(data,position) (*(reinterpret_cast<float *>(&(data[position]))))
#define BitConverter_ToUInt16(data,position) (*(reinterpret_cast<quint16 *>(&(data[position]))))
#define BitConverter_ToInt16(data,position) (*(reinterpret_cast<qint16 *>(&(data[position]))))

#define UDP_NMEA_PORT 9999

const QHostAddress epAgIO = QHostAddress("127.255.255.255");
const int epAgIO_port = 17777;

void FormGPS::ReceiveFromAgIO()
{
    double head253, rollK, Lat, Lon;

    /* this runs in the main GUI thread, it won't ever run at the
     * exact same time as the timer callback, so we can safely just
     * append to the rawBuffer, even if it's about to be processed
     * by the parser.
     */

    QByteArray datagram_data;

    //TODO: is this right or should we just work with one datagram at a time?
    datagram_data = udpSocket->receiveDatagram().data();

    while (udpSocket->hasPendingDatagrams()) {
        //receive all the data
        datagram_data.append(udpSocket->receiveDatagram().data());
    }

    char *data = datagram_data.data();

    if (datagram_data.length() > 4 && data[0] == (char)0x80 && data[1] == (char)0x81)
    {
        int Length = max((data[4]) + 5, 5);
        if (datagram_data.length() > Length)
        {
            char CK_A = 0;
            for (int j = 2; j < Length; j++)
            {
                CK_A += data[j];
            }

            if (data[Length] != (char)CK_A)
            {
                return;
            }
        }
        else
        {
            return;
        }

        switch ((uchar)data[3])
        {
        case 0xD6:
            if (udpWatch.elapsed() < udpWatchLimit)
            {
                udpWatchCounts++;
                //TODO implement nmea logging
                /*
                if (isLogNMEA) pn.logNMEASentence.Append("*** "
                               +  DateTime.UtcNow.ToString("ss.ff -> ", CultureInfo.InvariantCulture)
                               + udpWatch.ElapsedMilliseconds + "\r\n");
                */
                return;
            }
            udpWatch.restart();

            Lon = BitConverter_ToDouble(data, 5);
            Lat = BitConverter_ToDouble(data, 13);

            if (Lon != glm::DOUBLE_MAX && Lat != glm::DOUBLE_MAX)
            {
                if (timerSim.isActive())
                    DisableSim();

                pn.longitude = Lon;
                pn.latitude = Lat;

                pn.ConvertWGS84ToLocal(Lat, Lon, pn.fix.northing, pn.fix.easting);

                //From dual antenna heading sentences
                float temp = BitConverter_ToSingle(data, 21);
                if (temp != glm::DOUBLE_MAX)
                {
                    pn.headingTrueDual = temp + pn.headingTrueDualOffset;
                    if (pn.headingTrueDual >= 360) pn.headingTrueDual -= 360;
                    else if (pn.headingTrueDual < 0) pn.headingTrueDual += 360;
                    if (ahrs.isDualAsIMU) ahrs.imuHeading = pn.headingTrueDual;
                }

                //from single antenna sentences (VTG,RMC)
                pn.headingTrue = BitConverter_ToSingle(data, 25);

                //always save the speed.
                temp = BitConverter_ToSingle(data, 29);
                if (temp != glm::FLOAT_MAX)
                {
                    pn.vtgSpeed = temp;
                }

                //roll in degrees
                temp = BitConverter_ToSingle(data, 33);
                if (temp != glm::FLOAT_MAX)
                {
                    if (ahrs.isRollInvert) temp *= -1;
                    ahrs.imuRoll = temp - ahrs.rollZero;
                }
                if (temp == glm::FLOAT_MIN)
                    ahrs.imuRoll = 0;

                //altitude in meters
                temp = BitConverter_ToSingle(data, 37);
                if (temp != glm::FLOAT_MAX)
                    pn.altitude = temp;

                ushort sats = BitConverter_ToUInt16(data, 41);
                if (sats != glm::USHORT_MAX)
                    pn.satellitesTracked = sats;

                char fix = data[43];
                if (fix != glm::BYTE_MAX)
                    pn.fixQuality = fix;

                ushort hdop = BitConverter_ToUInt16(data, 44);
                if (hdop != glm::USHORT_MAX)
                    pn.hdop = hdop * 0.01;

                ushort age = BitConverter_ToUInt16(data, 46);
                if (age != glm::USHORT_MAX)
                    pn.age = age * 0.01;

                ushort imuHead = BitConverter_ToUInt16(data, 48);
                if (imuHead != glm::USHORT_MAX)
                {
                    ahrs.imuHeading = imuHead;
                    ahrs.imuHeading *= 0.1;
                }

                short imuRol = BitConverter_ToInt16(data, 50);
                if (imuRol != glm::SHORT_MAX)
                {
                    rollK = imuRol;
                    if (ahrs.isRollInvert) rollK *= -0.1;
                    else rollK *= 0.1;
                    rollK -= ahrs.rollZero;
                    ahrs.imuRoll = ahrs.imuRoll * ahrs.rollFilter + rollK * (1 - ahrs.rollFilter);
                }

                short imuPich = BitConverter_ToInt16(data, 52);
                if (imuPich != glm::SHORT_MAX)
                {
                    ahrs.imuPitch = imuPich;
                }

                short imuYaw = BitConverter_ToInt16(data, 54);
                if (imuYaw != glm::SHORT_MAX)
                {
                    ahrs.imuYawRate = imuYaw;
                }

                sentenceCounter = 0;

                //TODO: implement logging
                /*
                if (isLogNMEA)
                    pn.logNMEASentence.Append(
                        DateTime.UtcNow.ToString("mm:ss.ff",CultureInfo.InvariantCulture)+ " " +
                        Lat.ToString("N7") + " " + Lon.ToString("N7") );
                */
                UpdateFixPosition();
            }

            break;

        case 0xD3: //external IMU
            if (datagram_data.length() != 14)
                break;
            if (ahrs.imuRoll > 25 || ahrs.imuRoll < -25) ahrs.imuRoll = 0;
            //Heading
            ahrs.imuHeading = (qint16)((data[6] << 8) + data[5]);
            ahrs.imuHeading *= 0.1;

            //Roll
            rollK = (qint16)((data[8] << 8) + data[7]);

            if (ahrs.isRollInvert) rollK *= -0.1;
            else rollK *= 0.1;
            rollK -= ahrs.rollZero;
            ahrs.imuRoll = ahrs.imuRoll * ahrs.rollFilter + rollK * (1 - ahrs.rollFilter);

            //Angular velocity
            ahrs.angVel = (qint16)((data[10] << 8) + data[9]);
            ahrs.angVel /= -2;

            break;
        case 0xD4: //imu disconnect pgn
            if (data[5] == (char)1)
            {
                ahrs.imuHeading = 99999;

                ahrs.imuRoll = 88888;

                ahrs.angVel = 0;
            }
            break;
        case 253: //return from autosteer module
            //Steer angle actual
            if (datagram_data.length() != 14)
                break;
            mc.actualSteerAngleChart = (qint16)((data[6] << 8) + data[5]);
            mc.actualSteerAngleDegrees = (double)mc.actualSteerAngleChart * 0.01;

            //Heading
            head253 = (qint16)((data[8] << 8) + data[7]);
            if (head253 != 9999)
            {
                ahrs.imuHeading = head253 * 0.1;
            }

            //Roll
            rollK = (qint16)((data[10] << 8) + data[9]);
            if (rollK != 8888)
            {
                if (ahrs.isRollInvert) rollK *= -0.1;
                else rollK *= 0.1;
                rollK -= ahrs.rollZero;
                ahrs.imuRoll = ahrs.imuRoll * ahrs.rollFilter + rollK * (1 - ahrs.rollFilter);
            }
            //else ahrs.imuRoll = 88888;

            //switch status
            mc.workSwitchHigh = (data[11] & 1) == (char)1;
            mc.steerSwitchHigh = (data[11] & 2) == (char)2;

            //the pink steer dot reset
            steerModuleConnectedCounter = 0;

            //Actual PWM
            mc.pwmDisplay = data[12];

            //TODO implement NMEA logging
            /*
            if (isLogNMEA)
                pn.logNMEASentence.Append(
                    DateTime.UtcNow.ToString("mm:ss.ff", CultureInfo.InvariantCulture) + " AS " +
                    mc.actualSteerAngleDegrees.ToString("N1") + "\r\n"
                    );
            */
            break;

        case 250:
            if (datagram_data.length() != 14)
                break;
            mc.sensorData = data[5];
            break;

        case 234://MTZ8302 Feb 2020
            //Steer angle actual
            if (datagram_data.length() != 14)
                break;

            for (int i=0; i < 8 ; i++)
                mc.ss[i] = data[i+5];
            DoRemoteSwitches();

            break;
        }
    }
    //qDebug() << pn->rawBuffer ;
}

void FormGPS::DisableSim()
{
    isFirstFixPositionSet = false;
    isGPSPositionInitialized = false;
    isFirstHeadingSet = false;
    startCounter = 0;
    //TODO, turn off sim in UI
    //panelSim.Visible = false;
    //timerSim.Enabled = false;
    //simulatorOnToolStripMenuItem.Checked = false;
    property_setMenu_isSimulatorOn = false;
    //TODO, do we need to save explicitly?
    //Properties.Settings.Default.Save();
    return;
}

void FormGPS::StartLoopbackServer()
{
    AOGSettings s;
    int port = 15555; //property?

    if(udpSocket) stopUDPServer();

    udpSocket = new QUdpSocket(this); //should auto delete with the form
    udpSocket->bind(port); //by default, bind to all interfaces.
    //TODO: change to localhost

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(ReceiveFromAgIO()));
}

void FormGPS::stopUDPServer()
{
    if(udpSocket) {
        udpSocket->close();
        delete udpSocket;
        udpSocket = NULL;
    }
}

void FormGPS::SendPgnToLoop(QByteArray byteData) //10 bytes
{
    if (udpSocket != NULL && byteData.length() >2)
    {
        int crc = 0;
        for (int i = 2; i + 1 < byteData.length(); i++)
        {
            crc += byteData[i];
        }
        byteData[byteData.length() - 1] = (char)crc;
        udpSocket->writeDatagram(byteData,epAgIO,epAgIO_port);
    }
}
