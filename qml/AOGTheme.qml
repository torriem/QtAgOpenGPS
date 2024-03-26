import QtQuick 2.8
import QtQuick.Controls 2.5
import QtMultimedia 5.9

/* This type contains the sounds, colors, and perhaps screen sizes,
  *Sounds will track AOGInterface.qml, and react when needed as set
  by the features page.
  *Colors will follow the settings files, and change based on day/night
  mode, so we don't need an if statement in every object.
  *Screen sizes-We'll see.
  */

Item {
    id: aogTheme

    property color backgroundColor: "ghostWhite"
    property color textColor: "black"
    property color borderColor: "lightblue"
    property color blackDayWhiteNight: "black"

    Connections{
        target: settings
        function onSetDisplay_isDayModeChanged(){
            if (settings.setDisplay_isDayMode){
                backgroundColor = "ghostWhite"
                textColor = "black"
                borderColor = "lightBlue"
                blackDayWhiteNight = "black"
            }
            else{
                backgroundColor = "darkgray"
                textColor = "white"
                borderColor= "darkBlue"
                blackDayWhiteNight = "white"
            }
        }
    }

    Connections{//sounds functions go here.
        target: aog
        function onIsAutoSteerBtnOnChanged() {//will need another function for every sound option
            if(settings.setSound_isAutoSteerOn){//does the user want the sound on?
                if(aog.isAutoSteerBtnOn)
                    engage.play()
                else
                    disEngage.play()
            }
        }
    }
    SoundEffect{
        id: engage
        source: "/sounds/SteerOn.wav"
    }
    SoundEffect{
        id: disEngage
        source: "/sounds/SteerOff.wav"
    }
}
