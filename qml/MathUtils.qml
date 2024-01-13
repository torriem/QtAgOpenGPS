import QtQuick 2.0

Item {
    id: mathUtils

    //for testing
    AOGInterface {
        id: aog

    }

    function ConvertWGS84ToLocal( Lat, Lon) {
        var mPerDegreeLon = 111412.84 * cos(Lat * 0.01745329251994329576923690766743) - 93.5 * cos(3.0 * Lat * 0.01745329251994329576923690766743) + 0.118 * cos(5.0 * Lat * 0.01745329251994329576923690766743);
        var mPerDegreeLat = 111132.92 - 559.82 * cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                * cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                * cos(6.0 * latStart * 0.01745329251994329576923690766743);


        var outNorthing = (Lat - aog.latStart) * mPerDegreeLat;
        var outEasting = (Lon - aog.lonStart) * mPerDegreeLon;

        return [ outEasting, outNorthing ]
    }

    function ConvertLocalToWGS84(Northing, Easting)
    {
        var mPerDegreeLon = 111412.84 * cos(Lat * 0.01745329251994329576923690766743) - 93.5 * cos(3.0 * Lat * 0.01745329251994329576923690766743) + 0.118 * cos(5.0 * Lat * 0.01745329251994329576923690766743);
        var mPerDegreeLat = 111132.92 - 559.82 * cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                * cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                * cos(6.0 * latStart * 0.01745329251994329576923690766743);

        var outLat = ((Northing + fixOffset.northing) / mPerDegreeLat) + latStart;
        var outLon = ((Easting + fixOffset.easting) / mPerDegreeLon) + lonStart;

        return [ outLat, outLon ]
    }

}
