#include "cpgn.h"
#include "aogproperty.h"

CPGN_FE p_254;
CPGN_FC p_252;
CPGN_FB p_251;
CPGN_EF p_239;
CPGN_EE p_238;
CPGN_EC p_236;
CPGN_EB p_235;



CPGN_FC::CPGN_FC(): pgn(QByteArray( "\x80\x81\x7f\xfc\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 14))
{
    loadSettings();
}

void CPGN_FC::loadSettings()
{
    pgn[gainProportional] = property_setAS_Kp;
    pgn[highPWM] = property_setAS_highSteerPWM;
    pgn[lowPWM] = property_setAS_lowSteerPWM;
    pgn[minPWM] = property_setAS_minSteerPWM;
    pgn[countsPerDegree] = property_setAS_countsPerDegree;
    pgn[wasOffsetHi] = (char)((int)property_setAS_wasOffset >> 8);;
    pgn[wasOffsetLo] = (char)property_setAS_Kp;
    pgn[ackerman] = property_setAS_ackerman;
}

CPGN_EE::CPGN_EE() : pgn(QByteArray("\x80\x81\x7f\xee\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 14))
{
    loadSettings();
}

void CPGN_EE::loadSettings()
{
    pgn[raiseTime] = property_setArdMac_hydRaiseTime;
    pgn[lowerTime] = property_setArdMac_hydLowerTime;
    pgn[enableHyd] = property_setArdMac_isHydEnabled;
    pgn[set0] = property_setArdMac_setting0;

    pgn[user1] = property_setArdMac_user1;
    pgn[user2] = property_setArdMac_user2;
    pgn[user3] = property_setArdMac_user3;
    pgn[user4] = property_setArdMac_user4;
}

void CPGN_EE::MakeCRC()
{
    int crc = 0;
    for (int i = 2; i < pgn.length() - 1; i++)
    {
        crc += pgn[i];
    }
    pgn[pgn.length() - 1] = crc;
}

CPGN_EC::CPGN_EC() : pgn(QByteArray("\x80\x81\x7f\xec\x18\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 30))
{
    loadSettings();
}

void CPGN_EC::loadSettings()
{
    QVector<int> words = property_setRelay_pinConfig;

    pgn[pin0] = words[0];
    pgn[pin1] = words[1];
    pgn[pin2] = words[2];
    pgn[pin3] = words[3];
    pgn[pin4] = words[4];
    pgn[pin5] = words[5];
    pgn[pin6] = words[6];
    pgn[pin7] = words[7];
    pgn[pin8] = words[8];
    pgn[pin9] = words[9];

    pgn[pin10] = words[10];
    pgn[pin11] = words[11];
    pgn[pin12] = words[12];
    pgn[pin13] = words[13];
    pgn[pin14] = words[14];
    pgn[pin15] = words[15];
    pgn[pin16] = words[16];
    pgn[pin17] = words[17];
    pgn[pin18] = words[18];
    pgn[pin19] = words[19];

    pgn[pin20] = words[20];
    pgn[pin21] = words[21];
    pgn[pin22] = words[22];
    pgn[pin23] = words[23];
}

void CPGN_EC::MakeCRC()
{
    int crc = 0;
    for (int i = 2; i < pgn.length() - 1; i++)
    {
        crc += pgn[i];
    }
    pgn[pgn.length() - 1] = crc;
}

