import QtQuick 2.0

//a collection of functions to do unit and coordinate conversions

Item {
    id: unit_conversions
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

    function speed_to_unit_text(speed, decimal_places) {
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
            return qsTr("cm", "centimeter abbreviation")
        else
            return qsTr("inches", "inches unit")

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
            return qsTr("metre", "metre")
        else
            return qsTr("feet", "imperial feet")
    }


    function m_to_unit(distance) {
        if (isMetric())
            return distance
        else
            return distance * 3.2808 //convert to ft
    }

    function m_to_uniit_string(distance, decimal_places) {
        if (isMetric())
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance * 3.2808).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to ft
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


}
