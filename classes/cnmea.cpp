#include "cnmea.h"
#include "formgps.h"

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


CNMEA::CNMEA(FormGPS *mf)
    :mf(mf)
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
    if (dollar >= cr) rawBuffer = rawBuffer.mid(dollar);

    cr = rawBuffer.indexOf("\r\n");
    dollar = rawBuffer.indexOf("$");
    if (cr == -1 || dollar == -1) return;

    theSent = "";

    //now we have a complete sentence or more somewhere in the portData
    while (true)
    {
        //extract the next NMEA single sentence
        nextNMEASentence = parse();
        if (nextNMEASentence == "") return;

        //parse them accordingly
        words = nextNMEASentence.split(',');
        if (words.size() < 9) return;

        if ((words[0] == "$GPGGA") || (words[0] == "$GNGGA")) parseGGA();
        if ((words[0] == "$GPVTG") || (words[0] == "$GNVTG")) parseVTG();
        if ((words[0] == "$GPRMC") || (words[0] == "$GNRMC")) parseRMC();
    }// while still data


}

QByteArray CNMEA::parse() {
    QByteArray sentence;
    do
    {
        //double check for valid sentence
        // Find start of next sentence
        int start = rawBuffer.indexOf("$");
        if (start == -1) return "";
        rawBuffer = rawBuffer.mid(start);

        // Find end of sentence
        int end = rawBuffer.indexOf("\r\n");
        if (end == -1) return "";

        //the NMEA sentence to be parsed
        sentence = rawBuffer.mid(0, end + 2);

        //remove the processed sentence from the rawBuffer
        rawBuffer = rawBuffer.mid(end + 2);
    }

    //if sentence has valid checksum, its all good
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
    QByteArray sumStr = QByteArray::number(sum, 16).mid(0,2);

    // Compare to checksum in sentence
    return (sumStr == sentence.mid(inx + 1, 2));

    //TODO: catch exceptions here like the C# code did?
}

void CNMEA::parseGGA() {
    //$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M ,  ,*47
    //   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
    //        Time      Lat       Lon

    //is the sentence GGA
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
        mf->recvCounter = 0;
    }
}

void CNMEA::parseRMC() {
    //GPRMC parsing of the sentence
    //make sure there aren't missing coords in sentence
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
        //speed = Math.Round(speed * 1.852, 1);

        //True heading
        headingTrue = words[8].toDouble();

        //Status
        if (words[2].size()) {
            status = words[2];
        } else {
            status = "z";
        }

        theSent += nextNMEASentence;
        mf->recvCounter = 0;
        updatedRMC = true;

        mf->avgSpeed[mf->ringCounter] = speed;
        if (mf->ringCounter++ > 8) mf->ringCounter = 0;
    }

}

void CNMEA::parseVTG() {

}
