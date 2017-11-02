#ifndef TOPLINEDISPLAY_H
#define TOPLINEDISPLAY_H

#include <QWidget>

class QBoxLayout;
class QLabel;

class TopLineDisplay : public QWidget
{
    Q_OBJECT
public:
    QBoxLayout *box;
    QLabel *lblSpeed;
    QLabel *txtDistanceOffABLine;
    QLabel *lblSteerAngle;
    QLabel *lblHeading;
    QLabel *lblEasting;
    QLabel *lblNorthing;
    QLabel *lblZone;


    explicit TopLineDisplay(QWidget *parent = 0);


signals:

public slots:
};

#endif // TOPLINEDISPLAY_H
