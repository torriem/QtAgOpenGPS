import QtQuick
import QtQuick.Controls.Fusion
import QtMultimedia

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
        function onIsHydLiftDownChanged(){
            if(settings.setSound_isHydLiftOn){
                if(aog.isHydLiftDown)
                    hydDown.play()
                else
                    hydUp.play()
            }
        }
        function onAutoBtnStateChanged(){
            if(settings.setSound_isSectionsOn)
                sectionOn.play()
        }
        function onManualBtnStateChanged(){
            if(settings.setSound_isSectionsOn)
                sectionOff.play()
        }
        function onAgeChanged(){
            if(aog.age > settings.setGPS_ageAlarm)
                if(settings.setGPS_isRTK)
                    rtkLost.play()
        }
        function onDistancePivotToTurnLineChanged(){
            if(aog.distancePivotToTurnLine == 20)
                if(settings.setSound_isUturnOn)
                    approachingYouTurn.play()
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
    SoundEffect{
        id: hydDown
        source: "/sounds/HydDown.wav"
    }
    SoundEffect{
        id: hydUp
        source: "/sounds/HydUp.wav"
    }
    SoundEffect{
        id: sectionOff
        source: "/sounds/SectionOff.wav"
    }
    SoundEffect{
        id: sectionOn
        source: "/sounds/SectionOn.wav"
    }
    SoundEffect{
        id: approachingYouTurn
        source: "/sounds/Alarm10.wav"
    }
    SoundEffect{
        id: rtkLost
        source: "/sounds/rtk_lost.wav"
    }
    SoundEffect{
        id: youturnFail
        source: "/sounds/TF013.wav"
    }
}
