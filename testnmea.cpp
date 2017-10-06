#ifdef TEST_NMEA
#include <QApplication>
#include "formgps.h"
#include "cnmea.h"
#include <iostream>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    FormGPS mf(0);

    CNMEA cnmea(&mf);

    //throw it some bad data
    QByteArray nmea1("$GPGGA,225616.4,4952.005620,N,11143.030382,W,2,11,1.2,796.149,M,-17.686,M,1.0,007*76\r\n"
                     "$GPRMC,225616.4,A,4952.005620,asdfasdfsdf*10\r\n");
                     //"$GPRMC,225616.4,A,4952.005620,N,11143.030382,W,4.51,15.7,011116,0.0,E,D*10\r\n");

    cnmea.rawBuffer = nmea1;
    cnmea.parseNMEA();
    std::cout << cnmea.latitude << ", " << cnmea.longitude << std::endl;
    std::cout << cnmea.updatedGGA << ", " << cnmea.updatedRMC <<std::endl;
    //GGA should update but RMC won't.






}

#endif
