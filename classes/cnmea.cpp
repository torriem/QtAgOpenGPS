#include <QCoreApplication>
#include "cnmea.h"
//#include "latlong-utm.h"

//$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M ,  ,*47
//   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
//        Time      Lat       Lon

/*
GGA - essential fix data which provide 3D location and accuracy data.

 $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

Where:
     GGA          Global Positioning System Fix Data
     123519       Fix taken at 12:35:19 UTC
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     1            Fix quality: 0 = invalid
                               1 = GPS fix (SPS)
                               2 = DGPS fix
                               3 = PPS fix
                               4 = Real Time Kinematic
                               5 = Float RTK
                               6 = estimated (dead reckoning) (2.3 feature)
                               7 = Manual input mode
                               8 = Simulation mode
     08           Number of satellites being tracked
     0.9          Horizontal dilution of position
     545.4,M      Altitude, Meters, above mean sea level
     46.9,M       Height of geoid (mean sea level) above WGS84
                      ellipsoid
     (empty field) time in seconds since last DGPS update
     (empty field) DGPS station ID number
     *47          the checksum data, always begins with *
 *
 *
//$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
//  0      1    2   3      4    5      6   7     8     9     10   11
//        Time      Lat        Lon       knots  Ang   Date  MagV

Where:
     RMC          Recommended Minimum sentence C
     123519       Fix taken at 12:35:19 UTC
     A            Status A=active or V=Void.
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     022.4        Speed over the ground in knots
     084.4        Track angle in degrees True
     230394       Date - 23rd of March 1994
     003.1,W      Magnetic Variation
     *6A          The checksum data, always begins with *
 *
$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
 *
    VTG          Track made good and ground speed
    054.7,T      True track made good (degrees)
    034.4,M      Magnetic track made good
    005.5,N      Ground speed, knots
    010.2,K      Ground speed, Kilometers per hour
    *48          Checksum
*/
const double sm_a = 6378137.0;
const double sm_b = 6356752.314;
const double UTMScaleFactor = 0.9996;
//private double UTMScaleFactor2 = 1.0004001600640256102440976390556;


CNMEA::CNMEA()
{

}

void CNMEA::parseNMEA()
{
    if (!rawBuffer.size()) return;

    //find end of a sentence
    int cr = rawBuffer.indexOf("\r\n");
    if (cr == -1) return; // No end found, wait for more data

    // Find start of next sentence
    int dollar = rawBuffer.indexOf("$");
    if (dollar == -1) return;

    //if the $ isn't first, get rid of the tail of corrupt sentence
    if (dollar >= cr) rawBuffer = rawBuffer.mid(dollar);

    cr = rawBuffer.indexOf("\r\n");
    if (cr == -1) return; // No end found, wait for more data

    dollar = rawBuffer.indexOf("$");
    if (dollar == -1) return;

    //if the $ isn't first, get rid of the tail of corrupt sentence
    if (dollar >= cr)
        rawBuffer = rawBuffer.mid(dollar);

    cr = rawBuffer.indexOf("\r\n");
    dollar = rawBuffer.indexOf("$");
    if (cr == -1 || dollar == -1)
        return;

    theSent = "";

    //now we have a complete sentence or more somewhere in the portData
    while (true)
    {
        //extract the next NMEA single sentence
        nextNMEASentence = parse();
        if (nextNMEASentence == "") return;

        //parse them accordingly
        words = nextNMEASentence.split(',');
        if (words.size() < 9)
            return;

        if ((words[0] == "$GPGGA") || (words[0] == "$GNGGA")) parseGGA();
        if ((words[0] == "$GPVTG") || (words[0] == "$GNVTG")) parseVTG();
        if ((words[0] == "$GPRMC") || (words[0] == "$GNRMC"))
            parseRMC();
        //pump the main loop in case UDP, Serial, or TCP data comes in.
        QCoreApplication::processEvents();
    }// while still data


}

QByteArray CNMEA::parse() {
    QByteArray sentence;
    do
    {
        //double check for valid sentence
        // Find start of next sentence
        int start = rawBuffer.indexOf("$");
        if (start == -1)
            return "";

        rawBuffer = rawBuffer.mid(start);

        // Find end of sentence
        int end = rawBuffer.indexOf("\r\n");
        if (end == -1)
            return "";

        //the NMEA sentence to be parsed
        sentence = rawBuffer.mid(0, end + 2);

        //remove the processed sentence from the rawBuffer
        rawBuffer = rawBuffer.mid(end + 2);
    }

    //if sentence has valid checksum, its all good
    //TODO: what's with this while loop?
    //ask Brian.
    while (!validateChecksum(sentence));

    // Remove trailing checksum and \r\n and return
    sentence = sentence.mid(0, sentence.indexOf("*"));
    return sentence;
}

bool CNMEA::validateChecksum(QByteArray sentence) {
    int sum = 0;
    const char *sentenceChars = sentence.data();
    // All character xor:ed results in the trailing hex checksum
    // The checksum calc starts after '$' and ends before '*'
    int inx;
    char tmp;
    for (inx = 1; ; inx++)
    {
        if (inx >= sentence.size()) // No checksum found
            return false;
        tmp = sentenceChars[inx];
        // Indicates end of data and start of checksum
        if (tmp == '*') break;
        sum ^= tmp;    // Build checksum
    }

    // Calculated checksum converted to a 2 digit hex string
    QByteArray sumStr = QByteArray::number(sum, 16).toUpper();
    if (sumStr.length() < 2)
        sumStr.prepend('0');

    //not needed because checksum is only 8 bit and will always
    //be only 2 characters
    //sumStr = QByteArray::number(sum, 16).mid(0,2);

    // Compare to checksum in sentence
    return (sumStr == sentence.mid(inx + 1, 2).toUpper());

    //TODO: catch exceptions here like the C# code did?
}

void CNMEA::parseGGA() {
    //$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M ,  ,*47
    //   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
    //        Time      Lat       Lon

    //is the sentence GGA
    //qDebug() << "Parse GGA.";
    if (words[2].size() && words[3].size() &&
        words[4].size() && words[4].size() ) {
            //get latitude and convert to decimal degrees

            //TODO: check error status from toDouble()
            latitude = words[2].mid(0,2).toDouble();
            double temp = words[2].mid(2).toDouble();
            temp *= 0.01666666666666666666666666666667;
            latitude += temp;
            if (words[3] == "S")
            {
                latitude *= -1;
                hemisphere = 'S';
            }
            else hemisphere = 'N';

            //get longitude and convert to decimal degrees
            longitude = words[4].mid(0,3).toDouble();
            temp = words[4].mid(3).toDouble();
            longitude = longitude + temp * 0.01666666666666666666666666666667;

         { if (words[5] == "W") longitude *= -1; }

        //calculate zone and UTM coords
        decDeg2UTM();

        //fixQuality
        fixQuality = words[6].toInt();

        //satellites tracked
        satellitesTracked = words[7].toInt();

        //hdop
        hdop = words[8].toDouble();

        //altitude
        altitude = words[9].toDouble();

        //age of differential
        ageDiff = words[12].toDouble();

        theSent += nextNMEASentence;
        updatedGGA = true;
    }
}

void CNMEA::parseRMC() {
    //GPRMC parsing of the sentence
    //make sure there aren't missing coords in sentence
    //qDebug() << "Parse RMC.";
    if (words[3].size() && words[4].size() &&
        words[5].size() && words[6].size()) {
        //get latitude and convert to decimal degrees
        latitude = words[3].mid(0,2).toDouble();
        double temp;
        temp = words[3].mid(2).toDouble();
        latitude = latitude + temp * 0.01666666666666666666666666666667;

        if (words[4] == "S")
        {
                latitude *= -1;
                hemisphere = 'S';
        }
            else hemisphere = 'N';

        //get longitude and convert to decimal degrees
        longitude = words[5].mid(0,3).toDouble();
        temp = words[5].mid(3).toDouble();
        longitude = longitude + temp * 0.01666666666666666666666666666667;

        if (words[6] == "W") longitude *= -1;

        //calculate zone and UTM coords
        decDeg2UTM();

        //Convert from knots to kph for speed
        speed = words[7].toDouble();
        //round it when displaying, not here
        speed = speed * 1.852;

        //True heading
        headingTrue = words[8].toDouble();

        //Status
        if (words[2].size()) {
            status = words[2];
        } else {
            status = "z";
        }

        theSent += nextNMEASentence;
        updatedRMC = true;

    }

}

void CNMEA::parseVTG() {
    //$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
    //is the sentence GGA
    //qDebug() << "Parse VTG.";
    if (words[1].size() && words[5].size()) {
        //kph for speed - knots read
        speed = words[5].toDouble();
        speed = speed * 1.852;

        //True heading
        headingTrue = words[1].toDouble();

        updatedVTG = true;
        theSent += nextNMEASentence;
    }
}

double CNMEA::distance(double northing1, double easting1, double northing2, double easting2) {
    return sqrt( (easting1 - easting2) * (easting1 - easting2) +
                 (northing1 - northing2) * (northing1 - northing2));
}

double CNMEA::distanceSquared(double northing1, double easting1, double northing2, double easting2) {
    return (easting1 - easting2) * (easting1 - easting2) +
           (northing1 - northing2) * (northing1 - northing2);
}

void CNMEA::decDeg2UTM() {
    zone = floor( (longitude + 180.0) / 6) + 1;
    geoUTMConverterXY(latitude * 0.01745329251994329576923690766743,
                      longitude * 0.01745329251994329576923690766743);

    /*
    qDebug ()<< qSetRealNumberPrecision(10) <<
              latitude << ", " <<
              longitude << "=> " <<
              actualNorthing << ", " <<
              actualEasting << ", zone: " << zone;
    */
}

double CNMEA::arcLengthOfMeridian(double phi) {
    const double n = (sm_a - sm_b) / (sm_a + sm_b);
    double alpha = ((sm_a + sm_b) / 2.0) * (1.0 + (pow(n, 2.0) / 4.0) + (pow(n, 4.0) / 64.0));
    double beta = (-3.0 * n / 2.0) + (9.0 * pow(n, 3.0) / 16.0) + (-3.0 * pow(n, 5.0) / 32.0);
    double gamma = (15.0 * pow(n, 2.0) / 16.0) + (-15.0 * pow(n, 4.0) / 32.0);
    double delta = (-35.0 * pow(n, 3.0) / 48.0) + (105.0 * pow(n, 5.0) / 256.0);
    double epsilon = (315.0 * pow(n, 4.0) / 512.0);
    return alpha * (phi + (beta * sin(2.0 * phi))
            + (gamma * sin(4.0 * phi))
            + (delta * sin(6.0 * phi))
            + (epsilon * sin(8.0 * phi)));

}

XY CNMEA::mapLatLonToXY(double phi, double lambda, double lambda0) {
    XY xy;
    //double tmp;
    double ep2 = (pow(sm_a, 2.0) - pow(sm_b, 2.0)) / pow(sm_b, 2.0);
    double nu2 = ep2 * pow(cos(phi), 2.0);
    double n = pow(sm_a, 2.0) / (sm_b * sqrt(1 + nu2));
    double t = tan(phi);
    double t2 = t * t;
    //tmp = (t2 * t2 * t2) - pow(t, 6.0);
    double l = lambda - lambda0;
    double l3Coef = 1.0 - t2 + nu2;
    double l4Coef = 5.0 - t2 + 9 * nu2 + 4.0 * (nu2 * nu2);
    double l5Coef = 5.0 - 18.0 * t2 + (t2 * t2) + 14.0 * nu2 - 58.0 * t2 * nu2;
    double l6Coef = 61.0 - 58.0 * t2 + (t2 * t2) + 270.0 * nu2 - 330.0 * t2 * nu2;
    double l7Coef = 61.0 - 479.0 * t2 + 179.0 * (t2 * t2) - (t2 * t2 * t2);
    double l8Coef = 1385.0 - 3111.0 * t2 + 543.0 * (t2 * t2) - (t2 * t2 * t2);

    /* Calculate easting (x) */
    xy.x = n * cos(phi) * l
        + (n / 6.0 * pow(cos(phi), 3.0) * l3Coef * pow(l, 3.0))
        + (n / 120.0 * pow(cos(phi), 5.0) * l5Coef * pow(l, 5.0))
        + (n / 5040.0 * pow(cos(phi), 7.0) * l7Coef * pow(l, 7.0));

    /* Calculate northing (y) */
    xy.y = arcLengthOfMeridian(phi)
        + (t / 2.0 * n * pow(cos(phi), 2.0) * pow(l, 2.0))
        + (t / 24.0 * n * pow(cos(phi), 4.0) * l4Coef * pow(l, 4.0))
        + (t / 720.0 * n * pow(cos(phi), 6.0) * l6Coef * pow(l, 6.0))
        + (t / 40320.0 * n * pow(cos(phi), 8.0) * l8Coef * pow(l, 8.0));

    return xy;
}

void CNMEA::geoUTMConverterXY(double lat, double lon) {
    XY xy = mapLatLonToXY(lat, lon, (-183.0 + (zone * 6.0)) * 0.01745329251994329576923690766743);

    xy.x = xy.x * UTMScaleFactor + 500000.0;
    xy.y = xy.y * UTMScaleFactor;
    if (xy.y < 0.0)
        xy.y = xy.y + 10000000.0;

    //keep a copy of actual easting and northings
    actualEasting = xy.x;
    actualNorthing = xy.y;

    easting = xy.x - utmEast;
    northing = xy.y - utmNorth;
}
