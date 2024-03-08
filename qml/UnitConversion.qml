import QtQuick 2.0

//a collection of functions to do unit and coordinate conversions

Item {
    id: unit_conversions

    function isTrue(value) {
        return (value === true ||
                value === 'true')
    }

    function findDirection(heading) {
        if (heading > 337.5 || heading < 22.5)
        {
            return qsTr("N","compass bearing")
        }
        if (heading > 22.5 && heading < 67.5)
        {
            return qsTr("NE","compass bearing");
        }
        if (heading > 67.5 && heading < 111.5)
        {
            return qsTr("E", "compass bearing");
        }
        if (heading > 111.5 && heading < 157.5)
        {
            return qsTr("SE", "compass bearing");
        }
        if (heading > 157.5 && heading < 202.5)
        {
            return qsTr("S", "compass bearing");
        }
        if (heading > 202.5 && heading < 247.5)
        {
            return qsTr("SW", "compass bearing");
        }
        if (heading > 247.5 && heading < 292.5)
        {
            return qsTr("W", "compass bearing");
        }
        if (heading > 292.5 && heading < 337.5)
        {
            return qsTr("NW", "compass bearing");
        }
        return qsTr("??", "unknown compass bearing");
    }

    function findTrackDirection(abHeading,tracknum) {
        var heading = 0.0

        //identify what direction is 90 degrees to our
        //present track
        console.debug(abHeading)

        if (tracknum > 0)
            heading = abHeading + 90
        else
            heading = abHeading - 90

        if (heading > 360)
            heading -= 360
        if (heading < 0)
            heading += 360

        //now get the name of that direction
        if (heading > 337.5 || heading < 22.5)
        {
            return qsTr("N","compass bearing")
        }
        if (heading > 22.5 && heading < 67.5)
        {
            return qsTr("NE","compass bearing");
        }
        if (heading > 67.5 && heading < 111.5)
        {
            return qsTr("E", "compass bearing");
        }
        if (heading > 111.5 && heading < 157.5)
        {
            return qsTr("SE", "compass bearing");
        }
        if (heading > 157.5 && heading < 202.5)
        {
            return qsTr("S", "compass bearing");
        }
        if (heading > 202.5 && heading < 247.5)
        {
            return qsTr("SW", "compass bearing");
        }
        if (heading > 247.5 && heading < 292.5)
        {
            return qsTr("W", "compass bearing");
        }
        if (heading > 292.5 && heading < 337.5)
        {
            return qsTr("NW", "compass bearing");
        }
        return qsTr("??", "unknown compass bearing");
    }

    function isMetric() {
        return (settings.setMenu_isMetric === true || settings.setMenu_isMetric === "true")
    }

    function speed_unit() {
        if (isMetric())
            return qsTr("km/h","abbreviation for kilometers per hour")
        else
            return qsTr("MPH", "abbreviation for miles per hour")
    }

    function speed_to_unit(speed) {
        if (isMetric())
            return speed
        else
            return speed * 0.62137 //convert to mph
    }

    function speed_to_unit_string(speed, decimal_places) {
        if (isMetric())
            return Number(speed).toLocaleString(Qt.locale(), 'f', decimal_places)
        else
            return Number(speed * 0.62137).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to mph
    }

    function speed_from_unit(speed) {
        if (isMetric())
            return speed
        else
            return speed * 1.609344 // mph to kph
    }

    function cm_unit_abbrev() {
        if (isMetric())
            return qsTr("cm", "centimeter abbreviation")
        else
            return qsTr("in", "inches abbreviation")
    }

    function cm_unit() {
        if (isMetric())
            return qsTr("Centimeter", "centimeter unit")
        else
            return qsTr("Inches", "inches unit")

    }

    function cm_to_unit(distance) {
        if (isMetric())
            return Math.round(distance * 100) //base unit is metres, convert to centimetres
        else
            return Math.round(distance * 39.370079) //convert metres to inches
    }

    function cm_to_unit_string(distance, decimal_places) {
        if (isMetric())
            return Number(Math.round(distance * 100)).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(Math.round(distance * 39.370079)).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to inches
    }

    function cm_from_unit(distance) {
        if (isMetric())
            return distance / 100 //convert to base unit of metres
        else
            return distance / 39.370079 //convert inches to metres
    }

    function m_unit_abbrev() {
        if (isMetric())
            return qsTr("m", "metre abbreviation")
        else
            return qsTr("ft", "imperial feet abbreviation")
    }

    function m_unit() {
        if (isMetric())
            return qsTr("metres")
        else
            return qsTr("feet", "imperial feet")
    }


    function m_to_unit(distance) {
        if (isMetric())
            return distance
        else
            return distance * 3.2808 //convert to ft
    }

    function m_to_unit_string(distance, decimal_places) {
        if (isMetric())
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance * 3.2808).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to ft
    }
    
    function m_to_ft_string(distance) {
        if (isMetric())
            return qsTr(Number(distance).toLocaleString(Qt.locale(), 'f', 2)+ "m")
        else{
            var ftwithDec = distance * 3.2808
            var ft = Math.floor(ftwithDec)
            var inch = (ftwithDec - ft) * 12
            return qsTr((Number(ft).toLocaleString(Qt.locale(), 'f', 0))+ "' "+ (Number(inch).toLocaleString(Qt.locale(), 'f', 0))+ '"')
        }
    }

    function m_from_unit(distance) {
        if (isMetric())
            return distance //base is metres
        else
            return distance / 3.2808 //convert ft to metres
    }

    function km_unit() {
        if (isMetric())
            return qsTr("km", "kilometre abbreviation")
        else
            return qsTr("mi", "imperial miles abbreviation")
    }

    function km_to_unit(distance) {
        if (isMetric())
            return distance / 1000 //base is metres
        else
            return distance / 1609.344 //convert metres to miles
    }

    function km_to_unit_string(distance, decimal_places) {
        if (isMetric())
            return Number(distance / 1000).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance / 1609.344).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to miles
    }

    function km_from_unit(distance)
    {
        if (isMetric())
            return distance * 1000 // convert to metres
        else
            return distance * 1609.344 //convert miles to metres
    }
    function km_to_mi(distance){
        if (isMetric()){
            return distance
        }else
            return distance *  0.62137
    }
    function mi_to_km(distance){
        if (isMetric())
            return distance
        else
            return distance /  0.62137
    }

    function area_unit()
    {
        if (isMetric())
            return qsTr("ha", "hectares abbreviation")
        else
            return qsTr("ac", "acres abbreviation")
    }

    function area_to_unit(msquared)
    {
        if (isMetric())
            return msquared * 0.0001 //10000 m^2 in a hectare
        else
            return msquared * 2.47104E-4 //convert to acres

    }

    function area_to_unit_string(msquared, decimal_places)
    {
        return Number(area_to_unit(msquared)).toLocaleString(Qt.locale(),'f', decimal_places)
    }

    function area_from_unit(ha_or_acres)
    {
        if (isMetric())
            return ha_or_acres * 10000
        else
            return ha_or_acres * 4046.8726
    }

    function areaRate_unit()
    {
        if(isMetric())
            return qsTr("ha/hr","Abbreviation for hectares per hour")
        else
            return qsTr("ac/hr","Abbreviation for acres per hour")
    }

    function areaRate_to_unit(msquaredperhour)
    {
        return area_to_unit(msquaredperhour)
    }

    function areaRate_to_unit_string(msquaredperhour, decimal_places)
    {
        return area_to_unit(msquaredperhour, decimal_places)
    }

    function areaRate_from_unit(areaRate)
    {
        return area_from_unit(areaRate)
    }

    function deg_to_radians(degrees){
        return degrees* Math.PI/180
    }

    function radians_to_deg(radians){
        return radians * 180 / Math.PI
    }

    function distanceLatLon(latitude1, longitude1, latitude2, longitude2)
    {
        //This is a haversine based distance calculation formula
        var EARTH_RADIUS = 6378137.00

        //This portion converts the current and destination GPS coords from decDegrees to Radians
        var lonR1 = longitude1 * Math.PI / 180
        var lonR2 = longitude2 * Math.PI / 180
        var latR1 = latitude1 * Math.PI / 180
        var latR2 = latitude2 * Math.PI / 180

        //This portion calculates the differences for the Radian latitudes and longitudes and saves them to variables
        var dlon = lonR2 - lonR1;
        var dlat = latR2 - latR1;

        //This portion is the Haversine Formula for distance between two points. Returned value is in metres
        var a = (Math.pow(Math.sin(dlat * 0.5),2)) + Math.cos(latR1) * Math.cos(latR2) * (Math.pow(Math.sin(dlon * 0.5),2));
        var e = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)) ;

        return EARTH_RADIUS * e;
    }

    function timeTillFinished()
    {
        if (aog.speedKph > 2)
        {
            var total_time = ((aog.areaBoundaryOuterLessInner - aog.workedAreaTotal) / 1000 /
                              (settings.setVehicle_toolWidth * aog.speedKph * 0.1));
            var hours = Math.floor(total_time);
            var minutes = (total_time - hours) * 60;

            aog.timeTilFinished = (Number(hours).toLocaleString(Qt.locale(), 'f', 0))+ ":" + (Number(minutes).toLocaleString(Qt.locale(), 'f', 0))
        }
        else{
            aog.timeTilFinished = "\u221E Hrs"
        }
    }

    function workRate() {
        if (isMetric())
            aog.workRate = (Number(settings.setVehicle_toolWidth * aog.speedKph * 0.1).toLocaleString(Qt.locale(), 'f', 2)) + " ha/hr";
        else
            aog.workRate =  (Number(settings.setVehicle_toolWidth * aog.speedKph * 0.2471).toLocaleString(Qt.locale(), 'f', 2)) + " ac/hr";
    }
    function percents (){
        aog.percentLeft = ((aog.areaBoundaryOuterLessInner - aog.workedAreaTotal) / aog.areaBoundaryOuterLessInner* 100)
    }
    function string_before_comma(inputString){
        var commaIndex = inputString.indexOf(",");
        if (commaIndex !== -1) {
            inputString = inputString.substring(0, commaIndex);
            return inputString
        }
    }
    function string_after_comma(inputString){
        var commaIndex = inputString.indexOf(",");
        if (commaIndex !== -1) {
            inputString = inputString.substring(commaIndex + 1);
            return inputString
        }
    }

    Timer{
        id: guiTimer
        interval: 1000
        repeat: true
        running: true
        onTriggered: {
            workRate()
            timeTillFinished()
            percents()
        }
    }

}
