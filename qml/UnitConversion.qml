import QtQuick 2.0

//a collection of functions to do unit and coordinate conversions

Item {
    id: unit_conversions
    property double mPerDegreeLat
    property double mPerDegreeLon

    function setLocalMetersPerDegree() {
        mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)

        mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)
    }

    function convertLocalToWGS84(northing, easting) { //note northing first here
        var mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)
        var mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)

        var outLat = (northing / mPerDegreeLat) + latStart;
        var outLon = (easting  / mPerDegreeLon) + lonStart;

        return [ outLat, outLon ]
    }

    function convertWGS84ToLocal(latitude, longitude) {
        var mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)
        var mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)

        var outNorthing = (latitude - latStart) * mPerDegreeLat;
        var outEasting = (longitude - lonStart) * mPerDegreeLon;

        return [outNorthing, outEasting]
    }

    function speed_unit() {
        if (settings.setMenu_isMetric === true)
            return qsTr("km/h","abbreviation for kilometers per hour")
        else
            return qsTr("MPH", "abbreviation for miles per hour")
    }

    function speed_to_unit(speed) {
        if (settings.setMenu_isMetric === true)
            return speed
        else
            return speed * 0.62137 //convert to mph
    }

    function speed_to_unit_text(speed, decimal_places) {
        if (settings.setMenu_isMetric === true)
            return Number(speed).toLocaleString(Qt.locale(), 'f', decimal_places)
        else
            return Number(speed * 0.62137).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to mph
    }

    function speed_from_unit(speed) {
        if (settings.setMenu_isMetric === true)
            return speed
        else
            return speed * 1.609344 // mph to kph
    }

    function cm_unit() {
        if(settings.setMenu_isMetric === true)
            return qsTr("cm", "centimeter abbreviation")
        else
            return qsTr("in", "inches abbreviation")
    }

    function cm_to_unit(distance) {
        if(settings.setMenu_isMetric === true)
            return distance * 100 //base unit is metres, convert to centimetres
        else
            return distance * 39.370079 //convert metres to inches
    }

    function cm_to_unit_string(distance, decimal_places) {
        if(settings.setMenu_isMetric === true)
            return Number(distance * 100).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance * 39.370079).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to inches
    }

    function cm_from_unit(distance) {
        if(settings.setMenu_isMetric === true)
            return distance / 100 //convert to base unit of metres
        else
            return distance / 39.370079 //convert inches to metres
    }

    function m_unit() {
        if(settings.setMenu_isMetric === true)
            return qsTr("m", "metre abbreviation")
        else
            return qsTr("ft", "imperial feet abbreviation")
    }

    function m_to_unit(distance) {
        if(settings.setMenu_isMetric === true)
            return distance
        else
            return distance * 3.2808 //convert to ft
    }

    function m_to_uniit_string(distance, decimal_places) {
        if(settings.setMenu_isMetric === true)
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance * 3.2808).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to ft
    }

    function m_from_unit(distance) {
        if(settings.setMenu_isMetric === true)
            return distance //base is metres
        else
            return distance / 3.2808 //convert ft to metres
    }

    function km_unit() {
        if(settings.setMenu_isMetric === true)
            return qsTr("km", "kilometre abbreviation")
        else
            return qsTr("mi", "imperial miles abbreviation")
    }

    function km_to_unit(distance) {
        if(settings.setMenu_isMetric === true)
            return distance / 1000 //base is metres
        else
            return distance / 1609.344 //convert metres to miles
    }

    function km_to_unit_string(distance, decimal_places) {
        if(settings.setMenu_isMetric === true)
            return Number(distance / 1000).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance / 1609.344).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to miles
    }

    function km_from_unit(distance)
    {
        if(settings.setMenu_isMetric === true)
            return distance * 1000 // convert to metres
        else
            return distance * 1609.344 //convert miles to metres
    }


}
