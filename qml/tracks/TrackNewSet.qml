import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.Models
import "../components"
import ".."
/*This is where the track is actually created*/

//region SetAB
//
Item{
	id: trackNewSet
	anchors.fill: parent
	function showAPlus() {
		setAPlus.visible = true
	}
	function showAB() {
		setAB.visible = true
	}
	function showABCurve() {
		setABCurve.visible = true
	}
	function showLatLonLatLon() {
		latLonLatLon.visible = true
	}
	function showLatLonHeading() {
		latLonHeading.visible = true
	}
	function showCircleTrack() {
		circleTrack.visible = true
	}
	function showFromKML() {
		loadFromKML.open()
	}
	//region setA+

	MoveablePopup {
		id: setAPlus
		visible: false
		width: 250 * theme.scaleWidth
		height: 350 * theme.scaleHeight
		GridLayout {
			id: setAPlusLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.leftToRight
			rows: 3
			columns: 2

			// pick left or right 
			IconButtonTransparent { 
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/BoundaryRight.png"
				onClicked: {
					if (icon.source == "/images/BoundaryRight.png") {
						icon.source = "/images/BoundaryLeft.png"
					} else {
						icon.source = "/images/BoundaryRight.png"
					}
				}
			}
			IconButtonTransparent {
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/LetterABlue.png"
				onClicked: {
					aPlusHeading.enabled = true
					aPlusHeading.text = (Number(utils.radians_to_deg(aog.heading)).toLocaleString(Qt.locale(), 'f', 4))
					btnAPlusOk.enabled = true
				}
			}
			NumberTextField {
				id: aPlusHeading
				enabled: false
				bottomVal: 0
				topVal: 360
				decimals: 4
				text: "0"
				Layout.columnSpan: 2
				implicitWidth: 200
				implicitHeight: 50
			}	
			IconButtonTransparent {
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/Cancel64.png"
				onClicked: setAPlus.visible = false
			}
			IconButtonTransparent {
				id: btnAPlusOk
				Layout.alignment: Qt.AlignCenter
				enabled: false
				icon.source: "/images/OK64.png"
				onClicked: {
					setAPlus.visible = false
					trackAddName.show("A+ "+aPlusHeading.text+"°")
				}
			}
		}
	}
	//endregion setA+
	//
	//region setAB
	MoveablePopup {
		id: setAB
		visible: false
		width: 250 * theme.scaleWidth
		height: 350 * theme.scaleHeight
		GridLayout {
			id: setABLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.leftToRight
			rows: 3
			columns: 2

			// pick left or right 
			IconButtonTransparent { 
				Layout.alignment: Qt.AlignCenter
				Layout.columnSpan: 2
				icon.source: "/images/BoundaryRight.png"
				onClicked: {
					if (icon.source == "/images/BoundaryRight.png") {
						icon.source = "/images/BoundaryLeft.png"
					} else {
						icon.source = "/images/BoundaryRight.png"
					}
				}
			}
			IconButtonTransparent {
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/LetterABlue.png"
				onClicked: {
					btnB.enabled = true
				}
			}
			IconButtonTransparent {
				id: btnB
				Layout.alignment: Qt.AlignCenter
				enabled: false
				icon.source: "/images/LetterBBlue.png"
				onClicked: btnABOk.enabled = true
			}
			IconButtonTransparent {
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/Cancel64.png"
				onClicked: setAB.visible = false
			}
			IconButtonTransparent {
				id: btnABOk
				Layout.alignment: Qt.AlignCenter
				enabled: false
				icon.source: "/images/OK64.png"
				onClicked: {
					setAB.visible = false
					trackAddName.show("AB "+"not added"+"°")
				}
			}
		}
	}
	//endregion setAB
	//
	//region setABCurve
	MoveablePopup {
		id: setABCurve
		visible: false
		width: 250 * theme.scaleWidth
		height: 350 * theme.scaleHeight
		GridLayout {
			id: setABCurveLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.leftToRight
			rows: 4
			columns: 2

			// pick left or right 
			IconButtonTransparent { 
				Layout.alignment: Qt.AlignCenter
				Layout.columnSpan: 2
				icon.source: "/images/BoundaryRight.png"
				onClicked: {
					if (icon.source == "/images/BoundaryRight.png") {
						icon.source = "/images/BoundaryLeft.png"
					} else {
						icon.source = "/images/BoundaryRight.png"
					}
				}
			}
			IconButtonTransparent {
				id: btnACurve
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/LetterABlue.png"
				onClicked: {
					btnBCurve.enabled = true
					btnRecord.enabled = true
				}
			}
			IconButtonTransparent {
				id: btnBCurve
				Layout.alignment: Qt.AlignCenter
				enabled: false
				icon.source: "/images/LetterBBlue.png"
				onClicked: {
					setABCurve.visible = false
					trackAddName.show("Cu "+aog.heading+"°")
				}
			}
			Text {
				Layout.alignment: Qt.AlignCenter
				Layout.columnSpan: 2
				text: qsTr("Status: Driving")
				color: theme.textColor
			}
			IconButtonTransparent {
				id: btnRecord
				Layout.alignment: Qt.AlignCenter
				enabled: false
				checkable: true
				icon.source: "/images/boundaryPause.png"
				iconChecked: "/images/BoundaryRecord.png"
				onCheckedChanged: checked ? btnBCurve.enabled = false : btnBCurve.enabled = true
			}
			IconButtonTransparent {
				Layout.alignment: Qt.AlignCenter
				icon.source: "/images/Cancel64.png"
				onClicked: setABCurve.visible = false
			}
		}
	}
	//endregion setABCurve
	//
	//region latLonLatLon
	MoveablePopup {
		id: latLonLatLon
		visible: false
		width: 350 * theme.scaleWidth
		height: 450 * theme.scaleHeight
		onVisibleChanged: {
			if (visible) {
				latPointA.text = parseFloat((aog.latitude).toFixed(7))
				lonPointA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				latPointB.text = parseFloat((aog.latitude).toFixed(7)) //aog.latitude
				lonPointB.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
			}
		}
		GridLayout {
			id: latLonLatLonLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.TopToBottom
			rows: 8
			columns: 2

			// send current position to point A
			IconButtonTransparent {
				icon.source: "/images/Config/ConS_ImplementAntenna.png"
				Layout.rowSpan: 3
				Layout.row: 2
				Layout.column: 0
				onClicked: {
					latPointA.text = parseFloat((aog.latitude).toFixed(7)) //aog.latitude
					lonPointA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				}
			}

			// send current position to point B
			IconButtonTransparent {
				icon.source: "/images/Config/ConS_ImplementAntenna.png"
				Layout.rowSpan: 3
				Layout.row: 5
				Layout.column: 0
				onClicked: {
					latPointB.text = parseFloat((aog.latitude).toFixed(7)) //aog.latitude 
					lonPointB.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				}
			}
			/*
			 * figure out rounding
			 * we don't want to convert to locale string
			 */
			IconButtonTransparent {
				icon.source: "/images/Cancel64.png"
				onClicked: latLonLatLon.visible = false
				Layout.row: 8
				Layout.column: 0
			}

			//this starts on the top of the right column
			NumberTextField {
				id: latPointA
				Layout.alignment: Qt.AlignRight
				Layout.row: 1
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Latitude (+- 90)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			Text {
				Layout.alignment: Qt.AlignRight
				Layout.column: 1
				Layout.row: 2
				text: qsTr("A")
			}
			NumberTextField {
				id: lonPointA
				Layout.alignment: Qt.AlignRight
				Layout.row: 3
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Longitude (+- 180)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			NumberTextField {
				id: latPointB
				Layout.alignment: Qt.AlignRight
				Layout.row: 5 // skip a row
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Latitude (+- 90)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			Text {
				Layout.alignment: Qt.AlignRight
				Layout.column: 1
				Layout.row: 6
				text: qsTr("B")
			}
			NumberTextField {
				id: lonPointB
				Layout.alignment: Qt.AlignRight
				Layout.row: 7
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Longitude (+- 180)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			IconButtonTransparent {
				icon.source: "/images/OK64.png"
				Layout.row: 8
				Layout.column: 1
				Layout.alignment: Qt.AlignRight
				onClicked: {
					latLonLatLon.visible = false
					trackAddName.show("AB "+NA+"°")
				}
			}
		}
	}
	//endregion latLonLatLon
	//
	//region latLonHeading
	MoveablePopup {
		id: latLonHeading
		visible: false
		width: 350 * theme.scaleWidth
		height: 450 * theme.scaleHeight
		onVisibleChanged: {
			if (visible) {
				latPointAA.text = parseFloat((aog.latitude).toFixed(7))
				lonPointAA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				latLonHeadingEntry.text = parseFloat((aog.heading).toFixed(4))	
			}
		}
		GridLayout {
			id: latLonHeadingLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.TopToBottom
			rows: 4
			columns: 2

			// send current position to point A
			IconButtonTransparent {
				icon.source: "/images/Config/ConS_ImplementAntenna.png"
				Layout.rowSpan: 2
				Layout.column: 0
				onClicked: {
					latPointAA.text = parseFloat((aog.latitude).toFixed(7)) //aog.latitude
					lonPointAA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				}
			}

			IconButtonTransparent {
				icon.source: "/images/Cancel64.png"
				onClicked: latLonHeading.visible = false
				Layout.row: 4
				Layout.column: 0
			}

			//this starts on the top of the right column
			NumberTextField {
				id: latPointAA
				Layout.alignment: Qt.AlignRight
				Layout.row: 1
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Latitude (+- 90)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			NumberTextField {
				id: lonPointAA
				Layout.alignment: Qt.AlignRight
				Layout.row: 2
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Longitude (+- 180)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			NumberTextField {
				id: latLonHeadingEntry
				bottomVal: 0
				topVal: 360
				decimals: 4
				text: "0"
				Layout.row: 3
				Layout.column: 1
				Layout.alignment: Qt.AlignRight
				implicitWidth: 200
				implicitHeight: 50
				TextLine {
					text: qsTr("Heading")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}	
			IconButtonTransparent {
				icon.source: "/images/OK64.png"
				Layout.row: 4
				Layout.column: 1
				Layout.alignment: Qt.AlignRight
				onClicked: {
					latLonHeading.visible = false
					trackAddName.show("A+ "+latLonHeading.text+"°")
				}
			}
		}
	}
	//endregion latLonHeading
	//
	//region circleTrack
	MoveablePopup {
		id: circleTrack
		visible: false
		width: 350 * theme.scaleWidth
		height: 450 * theme.scaleHeight
		onVisibleChanged: {
			if (visible) {
				latPointAAA.text = parseFloat((aog.latitude).toFixed(7))
				lonPointAAA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
			}
		}
		GridLayout {
			id: circleTrackLayout
			anchors.centerIn: parent
			width: parent.width *.9
			height: parent.height *.9
			flow: Grid.TopToBottom
			rows: 4
			columns: 2

			// send current position to point A
			IconButtonTransparent {
				icon.source: "/images/Config/ConS_ImplementAntenna.png"
				Layout.rowSpan: 2
				Layout.column: 0
				onClicked: {
					latPointAAA.text = parseFloat((aog.latitude).toFixed(7)) //aog.latitude
					lonPointAAA.text = parseFloat((aog.longitude).toFixed(7)) //aog.longitude
				}
			}

			IconButtonTransparent {
				icon.source: "/images/Cancel64.png"
				onClicked: circleTrack.visible = false
				Layout.row: 3
				Layout.column: 0
			}

			//this starts on the top of the right column
			NumberTextField {
				id: latPointAAA
				Layout.alignment: Qt.AlignRight
				Layout.row: 1
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Latitude (+- 90)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			NumberTextField {
				id: lonPointAAA
				Layout.alignment: Qt.AlignRight
				Layout.row: 2
				Layout.column: 1
				implicitWidth: 200 * theme.scaleWidth
				bottomVal: -90
				topVal: 90
				decimals: 7
				text: "0"
				TextLine {
					text: qsTr("Longitude (+- 180)")
					anchors.bottom: parent.top
					anchors.left: parent.left
				}
			}
			IconButtonTransparent {
				icon.source: "/images/OK64.png"
				Layout.row: 3
				Layout.column: 1
				Layout.alignment: Qt.AlignRight
				onClicked: {
					circleTrack.visible = false
					trackAddName.show("Cir")
				}
			}
		}
	}
	//endregion circleTrack
	//
	//region loadFromKML
	FileDialog {
		id: loadFromKML
		currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation) + "/QtAgOpenGPS/Fields"
		nameFilters: ["KML Files (*.kml)"]
	}
/*Dialog {
	id: abSetter
	width: 300
	modal: true
	height: 550
	//border.width: 1
	//border.color: "black"
	//z: 1
	function show() {
		abSetter.visible = true
	}

	standardButtons: "NoButton"
	property double a_easting
	property double a_northing
	property double a_longitude
	property double a_latitude
	property double b_easting
	property double b_northing
	property double b_latitude
	property double b_longitude
	property bool a_set
	property double heading //radians
	property double heading_degrees
	background: Rectangle{
		id: backgroundABSetter
		border.width: 1
		border.color: aog.blackDayWhiteNight
		color: aog.backgroundColor
		anchors.fill: parent
	}

	visible: false

	onVisibleChanged: {
		if (visible === true) {
			a_set = false;
			a_stuff.visible = true;
			b_stuff.visible = false
			headingSpinbox.value = 0
		}
	}

	onRejected: {
		abLinePickerDialog.visible = true
	}

	onAccepted: {
		//emit signal to create the line
		//abLinePickerDialog.addLine(
			linesInterface.abLine_addLine(newLineName.abLineName,a_easting, a_northing, heading)
			linesInterface.abLine_updateLines()
			var count = linesInterface.abLinesList.length
			//ablineView.currentIndex = count -1
			//aog.currentABLine = count - 1
			abSetter.close()
			abLinePickerDialog.open()
		}

		Rectangle {
			id: a_stuff
			anchors.left: parent.left
			anchors.right: parent.right
			height: childrenRect.height
			color: "transparent"

			Text {
				id: a_label
				text:  qsTr("Drive to AB line <b>start</b> point and press the A button or enter Latitude and Longitude.")
				wrapMode: Text.WordWrap
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.margins: 5
			}
			IconButtonTransparent{
				objectName: "a"
				id: a
				anchors.top: a_label.bottom
				anchors.left: parent.left
				anchors.margins: 5
				checkable: false
				icon.source: "/images/LetterABlue.png"
				onClicked: {
					abSetter.a_easting = aog.toolEasting
					abSetter.a_northing = aog.toolNorthing
					abSetter.heading = aog.toolHeading
					abSetter.heading_degrees = aog.toolHeading * 180 / Math.PI
					linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
					a_manual_latitude.set_without_onchange(aog.toolLatitude)
					a_manual_longitude.set_without_onchange(aog.toolLongitude)
					b_stuff.visible = true

					headingSpinbox.set_without_onchange(abSetter.heading_degrees)
				}
			}

			GridLayout {
				anchors.top: a_label.bottom
				anchors.left: a.right
				anchors.right: parent.right
				anchors.margins: 5
				columns: 2
				rows: 2

				Text {
					text: qsTr("Lat","Latitude abbreviation")
				}
				LatLonTextField {
					id: a_manual_latitude
					Layout.fillWidth: true
					placeholderText: qsTr("Latitude")

					onManualTextChanged: {
						if (a_manual_longitude.text != "") {
							b_stuff.visible = true
						}
					}

					onEditingFinished: {
						if (a_manual_longitude.text != "") {
							const [northing, easting] = aog.convertWGS84ToLocal(Number(a_manual_latitude.text), Number(a_manual_longitude.text))
							abSetter.a_easting = easting
							abSetter.a_northing = northing
							abSetter.heading = aog.toolHeading
							abSetter.heading_degrees = aog.toolHeading * 180 / Math.PI
							linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
						}
					}
				}
				Text {
					text: qsTr("Lon","Longitude abbreviation")
				}
				LatLonTextField {
					id: a_manual_longitude
					Layout.fillWidth: true
					placeholderText: qsTr("Longitude")
					onManualTextChanged: {
						if (a_manual_latitude.text != "") {
							b_stuff.visible = true;
						}
					}
					onEditingFinished: {
						if (a_manual_latitude.text != "") {
							const [northing, easting] = aog.convertWGS84ToLocal(Number(a_manual_latitude.text), Number(a_manual_longitude.text))
							abSetter.a_easting = easting
							abSetter.a_northing = northing
							linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
						}
					}
				}
			}
		}

		Rectangle {
			id: b_stuff
			anchors.top: a_stuff.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			color: "transparent"

			Text {
				id: b_label
				text:  qsTr("For an A+ line, enter the heading.  Otherwise drive to AB line <b>end</b> point and press the B button, or manually enter Latitude and Longitude.")
				wrapMode: Text.WordWrap
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.margins: 5
			}

			SpinBox{
				id: headingSpinbox
				objectName: "heading"
				from: 0
				to: 35999999
				stepSize: 10000
				value: 0
				editable: true

				property real realValue: value/ 100000
				property int decimals: 2

				property bool suppress_onchange: false

				function set_without_onchange(new_value) {
					suppress_onchange = true
					value = new_value * 100000
					suppress_onchange = false
				}

				anchors.top: b_label.bottom
				anchors.margins: 5
				anchors.horizontalCenter: parent.horizontalCenter

				onValueChanged: {
					if (! suppress_onchange) {
						abSetter.heading = value / 100000 * Math.PI / 180.0
						abSetter.heading_degrees = value / 100000

						linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)

						//calculate b latitude and longitude for the display
						//use 100m
						abSetter.b_easting = Math.cos(abSetter.heading) * 100 + abSetter.a_easting
						abSetter.b_northing = Math.sin(abSetter.heading) * 100 + abSetter.a_northing

						const [latitude, longitude] = aog.convertLocalToWGS84(abSetter.b_northing, abSetter.b_easting)
						b_manual_latitude.set_without_onchange(latitude)
						b_manual_longitude.set_without_onchange(longitude)
					}
				}

				textFromValue: function(value, locale) {
					return Number(value / 100000).toLocaleString(locale, 'f', decimals)
				}

				valueFromText: function(text, locale) {
					return Number.fromLocaleString(locale, text) * 100000
				}
			}

			IconButtonTransparent{
				objectName: "b"
				id: b
				anchors.top: headingSpinbox.bottom
				anchors.left: parent.left
				anchors.margins: 5
				checkable: false
				icon.source: "/images/LetterBBlue.png"

				onClicked: {
					abSetter.b_easting = aog.toolEasting
					abSetter.b_northing = aog.toolNorthing

					b_manual_latitude.set_without_onchange(aog.toolLatitude)
					b_manual_longitude.set_without_onchange(aog.toolLongitude)

					abSetter.heading = Math.atan2(abSetter.b_easting - abSetter.a_easting,
					abSetter.b_northing - abSetter.a_northing)

					if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

					abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

					headingSpinbox.set_without_onchange(abSetter.heading_degrees)
				}
			}
			GridLayout {
				anchors.top: headingSpinbox.bottom
				anchors.left: b.right
				anchors.right: parent.right
				anchors.margins: 5
				columns: 2
				rows: 2

				Text {
					text: qsTr("Lat","Latitude abbreviation")
				}
				LatLonTextField {
					id: b_manual_latitude
					Layout.fillWidth: true
					placeholderText: qsTr("Latitude")
					onEditingFinished: {
						if (b_manual_longitude.text != "") {
							const [northing, easting] = aog.convertWGS84ToLocal(Number(b_manual_latitude.text), Number(b_manual_longitude.text))
							abSetter.b_easting = easting
							abSetter.b_northing = northing

							abSetter.heading = Math.atan2(abSetter.b_easting - abSetter.a_easting,
							abSetter.b_northing - abSetter.a_northing)

							if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

							abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

							headingSpinbox.set_without_onchange(abSetter.heading_degrees)
						}
					}
				}

				Text {
					text: qsTr("Lon","Longitude abbreviation")
				}
				LatLonTextField {
					id: b_manual_longitude
					Layout.fillWidth: true
					placeholderText: qsTr("Latitude")
					onEditingFinished: {
						if (b_manual_latitude.text != "") {
							const [northing, easting] = aog.convertWGS84ToLocal(Number(b_manual_latitude.text), Number(b_manual_longitude.text))
							abSetter.b_easting = easting
							abSetter.b_northing = northing
							if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

							abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

							headingSpinbox.set_without_onchange(abSetter.heading_degrees)
						}
					}
				}
			}
		}


		IconButtonTransparent{
			objectName: "btnCancel"
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.margins: 10
			icon.source: "/images/Cancel64.png"
			onClicked:{
				//cancel
				linesInterface.abLine_setA(false,0,0,0) //turn off line setting
				abSetter.rejected()
				abSetter.close()
			}
		}
		IconButtonTransparent{
			objectName: "btnOk"
			anchors.bottom: parent.bottom
			anchors.right: parent.right
			anchors.margins: 10
			icon.source: "/images/OK64.png"
			onClicked: {
				newLineName.visible = true
				newLineName.generate_ab_name(abSetter.heading_degrees)
				abSetter.visible = false
			}
		}
		LineName{
			id: newLineName
			objectName: "newLineName"
			anchors.top: parent.top
			anchors.left: parent.left
			title: "AB Line"
			visible: false
			onRejected: {
				//go back to A/B dialog
				//do nothing
				abSetter.visible = true
			}

			onAccepted: {
				abSetter.accepted()
			}
		}
	}

	LineName{
		id: copyLineName
		objectName: "copyLineName"
		anchors.top:parent.top
		anchors.left: parent.left
		title: "Copy AB Line"
		visible: false
		z: 2
		onAccepted: {
			if(ablineView.currentIndex > -1) {
				var heading = linesInterface.abLinesList[ablineView.currentIndex].heading
				var easting = linesInterface.abLinesList[ablineView.currentIndex].easting
				var northing = linesInterface.abLinesList[ablineView.currentIndex].northing
				linesInterface.abLine_addLine(copyLineName.abLineName, easting, northing, heading)
				//linesInterface.abLine_updateLines()
				ablineView.positionViewAtEnd()
			}
		}
	}

	LineName{
		id: editLineName
		objectName: "editLineName"
		anchors.top:parent.top
		anchors.left: parent.left
		title: "AB Line"
		visible: false
		z: 1
		onAccepted: {
			if(ablineView.currentIndex > -1) {
				linesInterface.abLine_changeName(ablineView.currentIndex, editLineName.abLineName)
				//linesInterface.abLine_updateLines()
				//ablineView.positionViewAtEnd()
			}
		}
	}

	Rectangle{
		id: listrect
		anchors.left: parent.left
		anchors.top:topLine.bottom
		anchors.right: rightColumn.left
		anchors.bottom: bottomRow.top
		anchors.bottomMargin: 0
		anchors.margins: 10
		color: aog.backgroundColor

		ListModel {
			id: ablineModel
			objectName: "ablineModel"
		}

		Component.onCompleted: {
			reloadModel()
		}

		ListView {
			id: ablineView
			anchors.fill: parent
			model: ablineModel
			//property int currentIndex: -1
			clip: true

			delegate: RadioButton{
				id: control
				checked: ablineView.currentIndex === index ? true : false
				indicator: Rectangle{
					anchors.fill: parent
					anchors.margins: 2
					//color: (control.down) ? aog.backgroundColor : aog.blackDayWhiteNight
					//color: (control.down) ? aog.blackDayWhiteNight : aog.backgroundColor
					color: control.checked ? "blue" : "white"
					visible: control.checked
				}

				onDownChanged: {
					ablineView.currentIndex = index
				}

				width:listrect.width
				height:50
				//anchors.fill: parent
				//color: "light gray"
				Text{
					anchors.left: parent.left
					anchors.leftMargin: 5
					anchors.verticalCenter: parent.verticalCenter
					text: model.name
					font.pixelSize: 25
					font.bold: true
					//color: control.checked ? aog.backgroundColor : aog.blackDayWhiteNight
					color: control.checked ? aog.blackDayWhiteNight : aog.backgroundColor
					z: 2
				}
			}
		}
	}
	*/
   }
