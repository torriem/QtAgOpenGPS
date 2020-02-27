#ifndef CSEQUENCE_H
#define CSEQUENCE_H

#include <QObject>
#include <QVector>
#include <QString>
#include "common.h"

class CYouTurn;

class SeqEvent
{
public:
    int function;
    int action;
    bool isTrig;
    double distance;

    SeqEvent();

};

class CSequence : public QObject
{
    Q_OBJECT
public:
    SeqEvent seqEnter[MAXFUNCTIONS];
    SeqEvent seqExit[MAXFUNCTIONS];

    QString pos1 = "Manual Button";
    QString pos2 = "Auto Button";
    QString pos3 = "";
    QString pos4 = "";
    QString pos5 = "";
    QString pos6 = "";
    QString pos7 = "";
    QString pos8 = "";

    int whereAmI = 0;
    bool isSequenceTriggered, isEntering;

    explicit CSequence(QObject *parent = 0);
    void DisableSeqEvent(int index, bool isEnter);
    void DoManualEntrySequence();
    void DoManualExitSequence();




signals:
    void doYouTurnSequenceEvent(int, int);
    void setDistanceToolToTurnLine(double);


public slots:
    void setIsSequenceTriggered(bool);
    void ResetSequenceEventTriggers();
    void ResetAllSequences();
    void DoSequenceEvent(CYouTurn &yt);
};

#endif // CSEQUENCE_H
