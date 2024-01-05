#ifndef CPGN_H
#define CPGN_H

#include <QByteArray>

//Latitude
class CPGN_D0
{
public:
    QByteArray latLong;

    CPGN_D0() :latLong(QByteArray( "\x80\x81\x7f\xd0\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC",14 )) { }
    void LoadLatitudeLongitude(double lat, double lon);
};

//AutoSteerData
class CPGN_FE
{
public:
    QByteArray pgn;
    int speedLo = 5;
    int speedHi = 6;
    int status = 7;
    int steerAngleLo = 8;
    int steerAngleHi = 9;
    int lineDistance  = 10;
    int sc1to8 = 11;
    int sc9to16 = 12;

    CPGN_FE() : pgn(QByteArray( "\x80\x81\x7f\xFE\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC",14 )) { }
    void Reset() { }
};

class CPGN_FD
{
public:
    QByteArray pgn;
    int actualLo = 5;
    int actualHi = 6;
    int headLo = 7;
    int headHi = 8;
    int rollLo = 9;
    int rollHi = 10;
    int switchStatus = 11;
    int pwm = 12;

    CPGN_FD() : pgn(QByteArray( "\x80\x81\x7f\xfd\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 14)) { }

    void Reset() { }
};

//AutoSteer Settings
class CPGN_FC
{
public:
    QByteArray pgn;
    int gainProportional = 5;
    int highPWM = 6;
    int lowPWM = 7;
    int minPWM = 8;
    int countsPerDegree = 9;
    int wasOffsetLo = 10;
    int wasOffsetHi = 11;
    int ackerman = 12;

    CPGN_FC();
    void loadSettings();
    void Reset();
};

//Autosteer Board Config
class CPGN_FB
{
public:
    QByteArray pgn;
    int set0 = 5;
    int maxPulse = 6;
    int minSpeed = 7;
    int set1 = 8;
    int angVel  = 9;

    CPGN_FB() : pgn(QByteArray( "\x80\x81\x7f\xfb\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 14)) { }
    void Reset() { }
};


// Machine Data
class CPGN_EF
{
public:
    QByteArray pgn;
    int uturn = 5;
    int speed = 6;
    int hydLift = 7;
    int tram = 8;
    int geoStop = 9; //out of bounds etc
    int sc1to8 = 11;
    int sc9to16 = 12;

    CPGN_EF() : pgn(QByteArray( "\x80\x81\x7f\xef\x08\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 14)) { }
    void Reset() { }
};

class CPGN_E5
{
public:
    QByteArray pgn;
    int sc1to8 = 5;
    int sc9to16 = 6;
    int sc17to24 = 7;
    int sc25to32 = 8;
    int sc33to40 = 9;
    int sc41to48 = 10;
    int sc49to56 = 11;
    int sc57to64 = 12;
    int toolLSpeed = 13;
    int toolRSpeed = 14;

    CPGN_E5() : pgn(QByteArray( "\x80\x81\x7f\xe5\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 16)) { }
    void Reset() { }
};

//Machine Config
class CPGN_EE
{
public:
    QByteArray pgn;
    int raiseTime = 5;
    int lowerTime = 6;
    int enableHyd = 7;
    int set0 = 8;
    int user1 = 9;
    int user2 = 10;
    int user3 = 11;
    int user4  = 12;

    // PGN  - 127.239 0x7FEF
    //int crc = 0;

    CPGN_EE();
    void loadSettings();
    void MakeCRC();
    void Reset() { }
};

//Relay Config
class CPGN_EC
{
public:
    QByteArray pgn;
    //where in the pgn is which pin
    int pin0 = 5;
    int pin1 = 6;
    int pin2 = 7;
    int pin3 = 8;
    int pin4 = 9;
    int pin5 = 10;
    int pin6 = 11;
    int pin7 = 12;
    int pin8 = 13;
    int pin9 = 14;

    int pin10 = 15;
    int pin11 = 16;
    int pin12 = 17;
    int pin13 = 18;
    int pin14 = 19;
    int pin15 = 20;
    int pin16 = 21;

    int pin17 = 22;
    int pin18 = 23;
    int pin19 = 24;
    int pin20 = 25;
    int pin21 = 26;
    int pin22 = 27;
    int pin23 = 28;

    // PGN  - 127.237 0x7FED
    CPGN_EC();
    void loadSettings();
    void MakeCRC();
    void Reset() {}
};

class CPGN_EB
{
public:
    QByteArray pgn;
    //where in the pgn is which pin
    int sec0Lo  = 5;
    int sec1Lo  = 7;
    int sec2Lo  = 9;
    int sec3Lo  = 11;
    int sec4Lo  = 13;
    int sec5Lo  = 15;
    int sec6Lo  = 17;
    int sec7Lo  = 19;
    int sec8Lo  = 21;
    int sec9Lo  = 23;
    int sec10Lo = 25;
    int sec11Lo = 27;
    int sec12Lo = 29;
    int sec13Lo = 31;
    int sec14Lo = 33;
    int sec15Lo = 35;

    int sec0Hi  = 6;
    int sec1Hi  = 8;
    int sec2Hi  = 10;
    int sec3Hi  = 12;
    int sec4Hi  = 14;
    int sec5Hi  = 16;
    int sec6Hi  = 18;
    int sec7Hi  = 20;
    int sec8Hi  = 22;
    int sec9Hi  = 24;
    int sec10Hi = 26;
    int sec11Hi = 28;
    int sec12Hi = 30;
    int sec13Hi = 32;
    int sec14Hi = 34;
    int sec15Hi = 36;

    int numSections = 37;

    CPGN_EB() : pgn(QByteArray( "\x80\x81\x7f\xeb\x21\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xCC", 39)) { }
    void Reset() {}
};

/*
extern CPGN_FE p_254;
extern CPGN_FC p_252;
extern CPGN_FB p_251;
extern CPGN_EF p_239;
extern CPGN_EE p_238;
extern CPGN_EC p_236;
extern CPGN_EB p_235;
extern CPGN_E5 p_229;
*/

#endif // CPGN_H
