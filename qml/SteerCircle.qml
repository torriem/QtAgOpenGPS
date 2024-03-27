import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: steerCircle
    color: "transparent"
    width: 120
    height: 120
    property color steerColor: "#F218ED"
    property double rollAngle: 0
    property font font: Qt.application.font

    Image {
        id: steerCircleImage
        anchors.fill: parent
        source: "/images/textures/z_SteerPointer.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    Colorize {
        id: colorize1
        anchors.fill: steerCircleImage
        source: steerCircleImage
        hue: 0.0
        saturation: 1
        lightness: -0.5

        transform: Rotation {
            origin.x: width / 2
            origin.y: height / 2
            angle: steerCircle.rollAngle
        }

    }

    Image {
        id: steerDotImage
        anchors.fill: parent
        source: "/images/textures/z_SteerDot.png"
        fillMode: Image.PreserveAspectCrop
        visible:false

    }

    Colorize {
        id: colorize2
        anchors.fill: steerDotImage
        source: steerDotImage
        hue: 0.0
        saturation: 1
        lightness: -0.5
    }

    Rectangle {
        width: t_metrics.width
        height: t_metrics.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        anchors.topMargin: (parent.height - height) / 2 - t_metrics.height

        //border.width: 1
        //border.color: steerColor

        color: "transparent"

        Text {
            anchors.fill:parent
            color: steerColor
            font: steerCircle.font

            id: rollAngleDisp
            text: Number(rollAngle).toLocaleString(Qt.locale(),'f',1)
        }

        TextMetrics {
            id: t_metrics
            font: rollAngleDisp.font
            text: rollAngleDisp.text
        }
    }

    onSteerColorChanged: {
        setRGB(steerColor.r, steerColor.g, steerColor.b);
    }

    Component.onCompleted: setRGB(steerColor.r, steerColor.g, steerColor.b)

    function setRGB (r, g, b) {
        let rabs, gabs, babs, rr, gg, bb, h, s, v, diff, diffc, percentRoundFn;
        rabs = r / 255;
        gabs = g / 255;
        babs = b / 255;
        v = Math.max(rabs, gabs, babs);
        diff = v - Math.min(rabs, gabs, babs);
        diffc = c => (v - c) / 6 / diff + 1 / 2;
        percentRoundFn = num => Math.round(num * 100) / 100;
        if (diff === 0) {
            h = s = 0;
        } else {
            s = diff / v;
            rr = diffc(rabs);
            gg = diffc(gabs);
            bb = diffc(babs);

            if (rabs === v) {
                h = bb - gg;
            } else if (gabs === v) {
                h = (1 / 3) + rr - bb;
            } else if (babs === v) {
                h = (2 / 3) + gg - rr;
            }
            if (h < 0) {
                h += 1;
            }else if (h > 1) {
                h -= 1;
            }
        }

        colorize2.hue = colorize1.hue = h;
        colorize2.saturation = colorize1.saturation = s;
        colorize2.lightness = colorize1.lightness = v - 0.5
    }
}
