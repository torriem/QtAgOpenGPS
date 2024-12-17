// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Mainly concerned with module communication on section related PGN data
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
        p_229.pgn[p_229.sc1to8] = p_254.pgn[p_254.sc1to8];
        p_229.pgn[p_229.sc9to16] = p_254.pgn[p_254.sc9to16];
        p_229.pgn[p_229.toolLSpeed] = (char)(tool.farLeftSpeed * 10);
        p_229.pgn[p_229.toolRSpeed] = (char)(tool.farRightSpeed * 10);
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

        p_239.pgn[p_239.sc1to8] = p_229.pgn[p_229.sc1to8];
        p_239.pgn[p_239.sc9to16] = p_229.pgn[p_229.sc9to16];

        p_254.pgn[p_254.sc1to8] = p_229.pgn[p_229.sc1to8];
        p_254.pgn[p_254.sc9to16] = p_229.pgn[p_229.sc9to16];

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


        if (tool.isSectionsNotZones)
        {
            if (mc.ss[mc.swOnGr0] != 0)
            {
                // ON Signal from Arduino
                if ((mc.ss[mc.swOnGr0] & 128) == 128 && tool.numOfSections > 7)
                {
                    tool.sectionButtonState.set(7,btnStates::On);
                    tool.section[7].sectionBtnState = btnStates::On;
                    //TODO: not sure why we have redundant states like that
                }
                if ((mc.ss[mc.swOnGr0] & 64) == 64 && tool.numOfSections > 6)
                {
                    tool.sectionButtonState.set(6,btnStates::On);
                    tool.section[6].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 32) == 32 && tool.numOfSections > 5)
                {
                    tool.sectionButtonState.set(5,btnStates::On);
                    tool.section[5].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 16) == 16 && tool.numOfSections > 4)
                {
                    tool.sectionButtonState.set(4,btnStates::On);
                    tool.section[4].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 8) == 8 && tool.numOfSections > 3)
                {
                    tool.sectionButtonState.set(3,btnStates::On);
                    tool.section[3].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 4) == 4 && tool.numOfSections > 2)
                {
                    tool.sectionButtonState.set(2,btnStates::On);
                    tool.section[2].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 2) == 2 && tool.numOfSections > 1)
                {
                    tool.sectionButtonState.set(1,btnStates::On);
                    tool.section[1].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr0] & 1) == 1)
                {
                    tool.sectionButtonState.set(0,btnStates::On);
                    tool.section[0].sectionBtnState = btnStates::On;
                }
                mc.ssP[mc.swOnGr0] = mc.ss[mc.swOnGr0];
            } //if swONLo != 0
            else { if (mc.ssP[mc.swOnGr0] != 0) { mc.ssP[mc.swOnGr0] = 0; } }

            if (mc.ss[mc.swOnGr1] != 0)
            {
                // sections ON signal from Arduino
                if ((mc.ss[mc.swOnGr1] & 128) == 128 && tool.numOfSections > 15)
                {
                    tool.sectionButtonState.set(15,btnStates::On);
                    tool.section[15].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 64) == 64 && tool.numOfSections > 14)
                {
                    tool.sectionButtonState.set(14,btnStates::On);
                    tool.section[14].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 32) == 32 && tool.numOfSections > 13)
                {
                    tool.sectionButtonState.set(13,btnStates::On);
                    tool.section[13].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 16) == 16 && tool.numOfSections > 12)
                {
                    tool.sectionButtonState.set(12,btnStates::On);
                    tool.section[12].sectionBtnState = btnStates::On;
                }

                if ((mc.ss[mc.swOnGr1] & 8) == 8 && tool.numOfSections > 11)
                {
                    tool.sectionButtonState.set(11,btnStates::On);
                    tool.section[11].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 4) == 4 && tool.numOfSections > 10)
                {
                    tool.sectionButtonState.set(10,btnStates::On);
                    tool.section[10].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 2) == 2 && tool.numOfSections > 9)
                {
                    tool.sectionButtonState.set(9,btnStates::On);
                    tool.section[9].sectionBtnState = btnStates::On;
                }
                if ((mc.ss[mc.swOnGr1] & 1) == 1 && tool.numOfSections > 8)
                {
                    tool.sectionButtonState.set(8,btnStates::On);
                    tool.section[8].sectionBtnState = btnStates::On;
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
                        if ((mc.ssP[mc.swOffGr0] & (1 << s)) && !(mc.ss[mc.swOffGr0] & (1 << s)) && (tool.sectionButtonState.get(s) == btnStates::Off))
                        {
                            tool.sectionButtonState.set(s,btnStates::Auto);
                            tool.section[s].sectionBtnState = btnStates::Auto;
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
                        if ((mc.ssP[mc.swOffGr1] & (1 << s)) && !(mc.ss[mc.swOffGr1] & (1 << s)) && (tool.sectionButtonState.get(s+8) == btnStates::Off))
                        {
                            tool.sectionButtonState.set(s+8,btnStates::Auto);
                            tool.section[s+8].sectionBtnState = btnStates::Auto;
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
                        tool.section[s].sectionBtnState = btnStates::Off;
                    }
                }
            } // if swOFFLo !=0
            if (mc.ss[mc.swOffGr1] != 0)
            {
                //if section SW in Arduino is switched to OFF; check always, if switch is locked to off GUI should not change
                for (int s=0; s<8; s++) {
                    if ((mc.ss[mc.swOffGr0] & (1 << s)) && tool.sectionButtonState.get(s+8) != btnStates::Off)
                    {
                        tool.sectionButtonState.set(s+8,btnStates::Off);
                        tool.section[s+8].sectionBtnState = btnStates::Off;
                    }
                }
            } // if swOFFHi !=0
        }//if serial or udp port open
        else
        {
            //DoZones
            int Bit;
            // zones to on
            if (mc.ss[mc.swOnGr0] != 0)
            {
                for (int i = 0; i < 8; i++)
                {
                    Bit = 1 << i;
                    if ((tool.zoneRanges[i + 1] > 0) && ((mc.ss[mc.swOnGr0] & Bit) == Bit))
                    {
                        tool.section[tool.zoneRanges[i + 1] - 1].sectionBtnState = btnStates::On;
                        tool.sectionButtonState.set(tool.zoneRanges[i + 1] - 1,btnStates::On);
                    }
                }

                mc.ssP[mc.swOnGr0] = mc.ss[mc.swOnGr0];
            }
            else { if (mc.ssP[mc.swOnGr0] != 0) { mc.ssP[mc.swOnGr0] = 0; } }

            // zones to auto
            if (mc.ss[mc.swOffGr0] != mc.ssP[mc.swOffGr0])
            {
                if (autoBtnState == btnStates::Auto)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        Bit = 1 << i;
                        if ((tool.zoneRanges[i + 1] > 0) && ((mc.ssP[mc.swOffGr0] & Bit) == Bit)
                            && ((mc.ss[mc.swOffGr0] & Bit) != Bit) && (tool.section[tool.zoneRanges[i + 1] - 1].sectionBtnState == btnStates::Off))
                        {
                        tool.section[tool.zoneRanges[i + 1] - 1].sectionBtnState = btnStates::Auto;
                        tool.sectionButtonState.set(tool.zoneRanges[i + 1] - 1,btnStates::Auto);

                        }
                    }
                }
                mc.ssP[mc.swOffGr0] = mc.ss[mc.swOffGr0];
            }

            // zones to off
            if (mc.ss[mc.swOffGr0] != 0)
            {
                for (int i = 0; i < 8; i++)
                {
                    Bit = 1 << i;
                    if ((tool.zoneRanges[i + 1] > 0) && ((mc.ss[mc.swOffGr0] & Bit) == Bit) && (tool.section[tool.zoneRanges[i + 1] - 1].sectionBtnState != btnStates::Off))
                    {
                        tool.section[tool.zoneRanges[i + 1] - 1].sectionBtnState = btnStates::Off;
                        tool.sectionButtonState.set(tool.zoneRanges[i + 1] - 1,btnStates::Off);
                    }
                }
            }
        }
    }
}
