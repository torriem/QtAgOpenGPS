#include "formgps.h"
#include "qmlutil.h"
#include "common.h"
#include "cpgn.h"


void FormGPS::btnSectionMasterManual_Click()
{
    qDebug()<<"Manual off on button clicked." ;

    //if Auto is on, turn it off
    autoBtnState = btnStates::Off;
    qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

    switch ((btnStates)manualBtnState)
    {
    case btnStates::Off:
        //roll to "on" state
        manualBtnState = btnStates::On;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",true);
        break;

    case btnStates::On:
        manualBtnState = btnStates::Off;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);
        break;
    case btnStates::Auto:
        break; //should never happen

    }

    //go set the butons and section states
    if (tool.isSectionsNotZones)
        AllSectionsAndButtonsToState(manualBtnState);
    else
        AllZonesAndButtonsToState(manualBtnState);

    closeAllMenus();
}

void FormGPS::btnSectionMasterAuto_Click(){
    qDebug()<<"Section off auto on button clicked." ;

    //turn off manual if on
    manualBtnState = btnStates::Off;
    qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

    switch ((btnStates)autoBtnState)
    {
    case btnStates::Off:
        autoBtnState = btnStates::Auto;
        qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",true);
        break;

    case btnStates::Auto:
        autoBtnState = btnStates::Off;
        qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);
        break;
    case btnStates::On:
        break; //do nothing
    }

    //go set the butons and section states
    if (tool.isSectionsNotZones)
        AllSectionsAndButtonsToState(autoBtnState);
    else
        AllZonesAndButtonsToState(autoBtnState);
    closeAllMenus();
}

//individual buttons for section (called by actual
//qt callback onSectionButton_clicked() SLOT
void FormGPS::btnSectionMan_Click(int sectNumber) {
    btnStates state = btnStates::Off;

    if(tool.isSectionsNotZones) {
        if (autoBtnState != btnStates::Auto) {
            //if auto is off just have on-off for choices of section buttons
            if (tool.section[sectNumber].sectionBtnState == btnStates::Off) {
                ///set to auto so that manuBtnUpdate will roll it over to On
                state = btnStates::On;
            }
        } else {
            switch(tool.section[sectNumber].sectionBtnState)
            {
            case btnStates::Off:
                state = btnStates::Auto;
                break;
            case btnStates::Auto:
                state = btnStates::On;
                break;
            case btnStates::On:
                state = btnStates::Off;
                break;
            }
        }
        IndividualSectionsAndButtonToState(state, sectNumber);
    } else {
        //Zones, so we have to set a bunch of functions
        QObject *button = qmlItem(qml_root,QString("section")+QString::number(sectNumber));
        QString old_state = button->property("state").toString();
        if (autoBtnState != btnStates::Auto) {
            //if auto is off just have on-off for choices of section buttons
            if (old_state == "off") {
                ///set to auto so that manuBtnUpdate will roll it over to On
                state = btnStates::On;
            }
        } else {
            if (old_state == "off")
                state = btnStates::Auto;
            else if (old_state == "auto")
                state = btnStates::On;
            else
                state = btnStates::Off;
        }
        IndividualZoneAndButtonToState(state,sectNumber);
    }

    //Roll over button to next state
    closeAllMenus(); //TODO get rid andput in qml
}

void FormGPS::AllSectionsAndButtonsToState(btnStates state)
{
    for (int i=0;i < 16; i++)
        IndividualSectionsAndButtonToState(state,i);
}

void FormGPS::IndividualSectionsAndButtonToState(btnStates state, int sectNumber)
{
    tool.section[sectNumber].sectionBtnState = state;
    QObject *button = qmlItem(qml_root,QString("section")+QString::number(sectNumber));

    switch(tool.section[sectNumber].sectionBtnState) {
    case btnStates::Off:
        tool.section[sectNumber].sectionBtnState = btnStates::Auto;
        button->setProperty("state","auto");
        break;
    case btnStates::Auto:
        tool.section[sectNumber].sectionBtnState = btnStates::On;
        button->setProperty("state","on");
        break;
    case btnStates::On:
        tool.section[sectNumber].sectionBtnState = btnStates::Off;
        button->setProperty("state","off");
        break;
    }
}

void FormGPS::LineUpIndividualSectionBtns()
{
    QObject* button;
    for (int b=0; b< 16; b++ ) {
        button = qmlItem(qml_root,QString("section")+QString::number(b));

        //temporarily enable them so we can test them
        button->setProperty("enabled", "true");
        if (b < tool.numOfSections) {
            button->setProperty("visible","true");
            if (isJobStarted)
                button->setProperty("enabled", "true");
        } else {
            button->setProperty("visible","false");
        }
    }
}

void FormGPS::LineUpAllZoneButtons()
{
    QObject* button;
    for (int b=0; b< 16; b++ ) {
        button = qmlItem(qml_root,QString("section")+QString::number(b));

        //temporarily enable them so we can test them
        button->setProperty("enabled", "true");
        if (b < tool.zones) {
            button->setProperty("visible","true");
            if (isJobStarted)
                button->setProperty("enabled", "true");
        } else {
            button->setProperty("visible","false");
        }
    }
}

void FormGPS::AllZonesAndButtonsToState(btnStates state)
{
    if (tool.zoneRanges.count() == 0) return;
    if (tool.zoneRanges[0] == 0) return;
    for (int i=0; i < tool.zoneRanges.count()-1; i++)
    {
        IndividualZoneAndButtonToState(state,i);
    }
}

void FormGPS::IndividualZoneAndButtonToState(btnStates state,int zoneNumber)
{
    QObject *button = qmlItem(qml_root,QString("section")+QString::number(zoneNumber));
    int sectionStartNumber, sectionEndNumber;

    if (zoneNumber == 0)
        sectionStartNumber = 0;
    else
        sectionStartNumber = tool.zoneRanges[zoneNumber];

    sectionEndNumber = tool.zoneRanges[zoneNumber+1];


    for (int i = sectionStartNumber; i < sectionEndNumber; i++)
    {
        tool.sectionButtonState.set(i,state);
    }

    switch(state) {
    case btnStates::Off:
        button->setProperty("state","auto");
        break;
    case btnStates::Auto:
        button->setProperty("state","on");
        break;
    case btnStates::On:
        button->setProperty("state","off");
        break;
    }
}

/* SectionSetPosition(), SectionCalcWidths(), and SectionCalcMulti() are all in CTool */

void FormGPS::BuildMachineByte()
{
    if (tool.isSectionsNotZones)
    {
        p_254.pgn[p_254.sc1to8] = 0;
        p_254.pgn[p_254.sc9to16] = 0;

        int number = 0;
        for (int j = 0; j < 8; j++)
        {
            if (tool.section[j].isSectionOn)
                number |= 1 << j;
        }
        p_254.pgn[p_254.sc1to8] = (char)number;
        number = 0;

        for (int j = 8; j < 16; j++)
        {
            if (tool.section[j].isSectionOn)
                number |= 1 << (j-8);
        }
        p_254.pgn[p_254.sc9to16] = (char)number;

        //machine pgn
        p_239.pgn[p_239.sc9to16] = p_254.pgn[p_254.sc9to16];
        p_239.pgn[p_239.sc1to8] = p_254.pgn[p_254.sc1to8];
    }
    else
    {
        //zero all the bytes - set only if on
        for (int i = 5; i < 13; i++)
        {
            p_229.pgn[i] = 0;
        }

        int number = 0;
        for (int k = 0; k < 8; k++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (tool.section[j + k * 8].isSectionOn)
                    number |= 1 << j;
            }
            p_229.pgn[5 + k] = (char)number;
            number = 0;
        }

        //tool speed to calc ramp
        p_229.pgn[p_229.toolLSpeed] = (char)(tool.farLeftSpeed * 10);
        p_229.pgn[p_229.toolRSpeed] = (char)(tool.farRightSpeed * 10);
    }

    p_239.pgn[p_239.speed] = (char)(vehicle.avgSpeed * 10);
    p_239.pgn[p_239.tram] = (char)tram.controlByte;
}

void FormGPS::DoRemoteSwitches()
{
    //MTZ8302 Feb 2020
    if (isJobStarted)
    {
        //MainSW was used
        if (mc.ss[mc.swMain] != mc.ssP[mc.swMain])
        {
            //Main SW pressed
            if ((mc.ss[mc.swMain] & 1) == 1)
            {
                //set butto off and then press it = ON
                autoBtnState = btnStates::Off;
                btnSectionMasterAuto_Click();
            } // if Main SW ON

            //if Main SW in Arduino is pressed OFF
            if ((mc.ss[mc.swMain] & 2) == 2)
            {
                //set button on and then press it = OFF
                autoBtnState = btnStates::Auto;
                btnSectionMasterAuto_Click();
            } // if Main SW OFF

            mc.ssP[mc.swMain] = mc.ss[mc.swMain];
        }  //Main or Rate SW


        if (mc.ss[mc.swOnGr0] != 0)
        {
            // ON Signal from Arduino
            for (int s=0; s < 8; s++){
                if((mc.ss[mc.swOnGr0] & (1 << s)) && tool.numOfSections > s) {
                    tool.sectionButtonState.set(s,btnStates::On);
                }
            }

            mc.ssP[mc.swOnGr0] = mc.ss[mc.swOnGr0];
        } //if swONLo != 0
        else { if (mc.ssP[mc.swOnGr0] != 0) { mc.ssP[mc.swOnGr0] = 0; } }

        if (mc.ss[mc.swOnGr1] != 0)
        {
            for (int s=0; s<8; s++) {
                if ((mc.ss[mc.swOnGr1] & (1 << s)) && tool.numOfSections > (s+8))
                {
                    tool.sectionButtonState.set(s+8, btnStates::On);
                }
            }
            mc.ssP[mc.swOnGr1] = mc.ss[mc.swOnGr1];
        } //if swONHi != 0
        else { if (mc.ssP[mc.swOnGr1] != 0) { mc.ssP[mc.swOnGr1] = 0; } }

        // Switches have changed
        if (mc.ss[mc.swOffGr0] != mc.ssP[mc.swOffGr0])
        {
            //if Main = Auto then change section to Auto if Off signal from Arduino stopped
            if (autoBtnState == btnStates::Auto)
            {

                for(int s=0; s< 8; s++) {
                    if ((mc.ssP[mc.swOffGr0] & (1 << s)) && (mc.ss[mc.swOffGr0] & (1 << s)) && (tool.sectionButtonState.get(s) == btnStates::Off))
                    {
                        tool.sectionButtonState.set(s,btnStates::Auto);
                    }
                }
            }
            mc.ssP[mc.swOffGr0] = mc.ss[mc.swOffGr0];
        }

        if (mc.ss[mc.swOffGr1] != mc.ssP[mc.swOffGr1])
        {
            //if Main = Auto then change section to Auto if Off signal from Arduino stopped
            if (autoBtnState == btnStates::Auto)
            {
                for(int s=8; s< 16; s++) {
                    if ((mc.ssP[mc.swOffGr1] & (1 << s)) && (mc.ss[mc.swOffGr1] & (1 << s)) && (tool.sectionButtonState.get(s+8) == btnStates::Off))
                    {
                        tool.sectionButtonState.set(s+8,btnStates::Auto);
                    }
                }
            }
            mc.ssP[mc.swOffGr1] = mc.ss[mc.swOffGr1];
        }

        // OFF Signal from Arduino
        if (mc.ss[mc.swOffGr0] != 0)
        {
            //if section SW in Arduino is switched to OFF; check always, if switch is locked to off GUI should not change
            for(int s=0; s< 8; s++) {
                if ((mc.ss[mc.swOffGr0] & (1 << s)) && tool.sectionButtonState.get(s) != btnStates::Off)
                {
                    tool.sectionButtonState.set(s,btnStates::Off);
                }
            }
        } // if swOFFLo !=0
        if (mc.ss[mc.swOffGr1] != 0)
        {
            //if section SW in Arduino is switched to OFF; check always, if switch is locked to off GUI should not change
            for (int s=0; s<8; s++) {
                if ((mc.ss[mc.swOffGr0] & (1 << s)) && tool.sectionButtonState.get(s+8) != btnStates::Off)
                {
                    tool.sectionButtonState.set(s,btnStates::Off);
                }
            }
        } // if swOFFHi !=0
    }//if serial or udp port open
}
