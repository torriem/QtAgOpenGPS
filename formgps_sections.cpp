#include "formgps.h"
#include "qmlutil.h"
#include "common.h"
#include "cpgn.h"


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
                autoBtnState = btnStates::Off;
            } // if Main SW ON

            //if Main SW in Arduino is pressed OFF
            if ((mc.ss[mc.swMain] & 2) == 2)
            {
                autoBtnState = btnStates::Auto;
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
