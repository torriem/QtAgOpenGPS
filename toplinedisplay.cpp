#include "toplinedisplay.h"
#include <QBoxLayout>
#include <QLabel>

TopLineDisplay::TopLineDisplay(QWidget *parent) : QWidget(parent)
{
    QWidget *thisWidget = dynamic_cast<QWidget *>(this);

    box = new QBoxLayout(QBoxLayout::LeftToRight,thisWidget);
    lblSpeed = new QLabel(thisWidget);
    lblSpeed->setText("Speed");

    txtDistanceOffABLine = new QLabel(thisWidget);
    txtDistanceOffABLine->setText("0000000");

    lblSteerAngle = new QLabel(thisWidget);
    lblSteerAngle->setText("Ang");

    lblHeading = new QLabel(thisWidget);
    lblHeading->setText("Dir");

    lblEasting = new QLabel(thisWidget);

    lblNorthing = new QLabel(thisWidget);
    lblZone = new QLabel(thisWidget);

    box->addWidget(lblSpeed);
    box->addWidget(txtDistanceOffABLine,1,Qt::AlignCenter);
    box->addWidget(lblSteerAngle);
    box->addWidget(lblHeading);
    box->addWidget(lblEasting);
    box->addWidget(lblNorthing);
    box->addWidget(lblZone);
    this->setLayout(box);
}
