#!/bin/bash
AOG_PATH=$1

echo "|$AOG_PATH|"

if [ ! -d "qml" ]; then
	echo "Please run this script from the root of the QtAgOpenGPS source code tree."
	exit 1
fi

if [ ! -n "$AOG_PATH" ]; then
	echo "Please pass the path to root of the AgOpenGPS project directory"
	echo "as a parameter to this script."
	exit 1
fi

echo "Generating aogproperties.cpp"
python3 parse_properties.py "$AOG_PATH/SourceCode/GPS/Properties/Settings.settings" "$AOG_PATH/SourceCode/GPS/Classes/CSettings.cs" -c > aogproperties.cpp
echo "Generating properties.h"
python3 parse_properties.py "$AOG_PATH/SourceCode/GPS/Properties/Settings.settings" "$AOG_PATH/SourceCode/GPS/Classes/CSettings.cs" -i > properties.h
echo "Generating qmlsettings_addkeys.cpp"
python3 parse_properties.py "$AOG_PATH/SourceCode/GPS/Properties/Settings.settings" "$AOG_PATH/SourceCode/GPS/Classes/CSettings.cs" -q  > qmlsettings_addkeys.cpp
echo "Generating qml/MockSettings.qml"
python3 parse_properties.py "$AOG_PATH/SourceCode/GPS/Properties/Settings.settings" "$AOG_PATH/SourceCode/GPS/Classes/CSettings.cs" -m > qml/MockSettings.qml    

