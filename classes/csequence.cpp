#include "csequence.h"
#include "aogsettings.h"
#include <QStringList>
#include "common.h"
#include "cyouturn.h"


CSequence::CSequence(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;

    //Fill in the strings for comboboxes - editable
    QString line = SETTINGS_VEHICLE_SEQFUNCTIONLIST;
    QStringList words = line.split(',');

    pos3 = words[0];
    pos4 = words[1];
    pos5 = words[2];
    pos6 = words[3];
    pos7 = words[4];
    pos8 = words[5];

    QString sentence;

    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        seqEnter[i].function = 0;
        seqEnter[i].action = 0;
        seqEnter[i].isTrig = true;
        seqEnter[i].distance = 0;
    }

    sentence = SETTINGS_VEHICLE_SEQFUNCTIONENTER;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqEnter[i].function = words[i].toInt();

    sentence = SETTINGS_VEHICLE_SEQACTIONENTER;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqEnter[i].action=words[i].toInt();

    sentence = SETTINGS_VEHICLE_SEQDISTANCEENTER;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqEnter[i].distance = words[i].toInt();

    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        seqExit[i].function = 0;
        seqExit[i].action = 0;
        seqExit[i].isTrig = true;
        seqExit[i].distance = 0;
    }

    sentence = SETTINGS_VEHICLE_SEQFUNCTIONEXIT;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqExit[i].function = words[i].toInt();

    sentence = SETTINGS_VEHICLE_SEQACTIONEXIT;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqExit[i].action = words[i].toInt();

    sentence = SETTINGS_VEHICLE_SEQDISTANCEEXIT;
    words = sentence.split(',');
    for (int i = 0; i < MAXFUNCTIONS; i++)
        seqExit[i].distance = words[i].toInt();

}

void CSequence::DisableSeqEvent(int index, bool isEnter)
{
    if (isEnter)
    {
        seqEnter[index].function = 0;
        seqEnter[index].action = 0;
        seqEnter[index].isTrig = true;
        seqEnter[index].distance = 0;
    }
    else
    {
        seqExit[index].function = 0;
        seqExit[index].action = 0;
        seqExit[index].isTrig = true;
        seqExit[index].distance = 0;
    }
}

void CSequence::ResetAllSequences()
{
    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        seqEnter[i].function = 0;
        seqEnter[i].action = 0;
        seqEnter[i].isTrig = true;
        seqEnter[i].distance = 0;
    }
    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        seqExit[i].function = 0;
        seqExit[i].action = 0;
        seqExit[i].isTrig = true;
        seqExit[i].distance = 0;
    }
}

void CSequence::ResetSequenceEventTriggers()
{
    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        if (seqEnter[i].function != 0) seqEnter[i].isTrig = false;
        if (seqExit[i].function != 0) seqExit[i].isTrig = false;
    }

    isSequenceTriggered = false;
}

void CSequence::DoSequenceEvent(CYouTurn &yt)
{
    if (isSequenceTriggered)
    {
        //determine if Section is entry or exit based on trigger point direction
        //bool isToolHeadingSameAsABHeading;

        if (yt.onA <= 0)
        {
            whereAmI = 1;
            yt.onA = -yt.onA;
        }
        else
        {
            whereAmI = 2;
        }

        int c = 0;
        for (int i = 0; i < MAXFUNCTIONS; i++)
        {
            //checked for any not triggered yet (false) - if there is, not done yet
            if (!seqEnter[i].isTrig) c++;
            if (!seqExit[i].isTrig) c++;
        }

        if (c == 0)
        {
            //sequences all done so reset everything and leave
            isSequenceTriggered = false;
            whereAmI = 0;
            ResetSequenceEventTriggers();
            emit setDistanceToolToTurnLine(-2222); //current unused
            //mf.distanceToolToTurnLine = -2222;
            isSequenceTriggered = false;
            return;
        }

        switch (whereAmI)
        {
            case 0: //not in you turn
                break;

            case 1: //Entering the headland

                for (int i = 0; i < MAXFUNCTIONS; i++)
                {
                    //have we gone past the distance and still haven't done it
                    if (yt.onA >= seqEnter[i].distance && !seqEnter[i].isTrig)
                    {
                        //it shall only run once
                        seqEnter[i].isTrig = true;

                        //send the function and action to perform
                        //this should fire immediately since we're in the same thread.
                        emit doYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
                        //mf.DoYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
                    }
                }
                break;

            case 2: //Exiting the headland

                for (int i = 0; i < MAXFUNCTIONS; i++)
                {
                    //have we gone past the distance and still haven't done it
                    if (yt.onA <= seqExit[i].distance && !seqExit[i].isTrig)
                    {
                        //it shall only run once
                        seqExit[i].isTrig = true;

                        //send the function and action to perform
                        //this should fire immediately since we're in the same thread.
                        emit doYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
                        //mf.DoYouTurnSequenceEvent(seqExit[i].function, seqExit[i].action);
                    }
                }
                break;
        }
    }
}

void CSequence::DoManualEntrySequence()
{
    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        //have we gone past the distance and still haven't done it
        if (!seqEnter[i].isTrig)
        {
            //it shall only run once
            seqEnter[i].isTrig = true;

            //send the function and action to perform
            //this should fire immediately since we're in the same thread.
            emit doYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
            //mf.DoYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
        }
    }
    ResetSequenceEventTriggers();
}

void CSequence::DoManualExitSequence()
{
    for (int i = 0; i < MAXFUNCTIONS; i++)
    {
        //have we gone past the distance and still haven't done it
        if (!seqExit[i].isTrig)
        {
            //it shall only run once
            seqExit[i].isTrig = true;

            //send the function and action to perform
            //this should fire immediately since we're in the same thread.
            emit doYouTurnSequenceEvent(seqEnter[i].function, seqEnter[i].action);
            //mf.DoYouTurnSequenceEvent(mf.seq.seqExit[i].function, mf.seq.seqExit[i].action);
        }
    }
    ResetSequenceEventTriggers();
}

void CSequence::setIsSequenceTriggered(bool _trig)
{
    //slot so that CYouTurn can call this indirectly
    //unused
    isSequenceTriggered = _trig;
}


