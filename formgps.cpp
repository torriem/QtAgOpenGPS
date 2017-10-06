#include "formgps.h"
#include "ui_formgps.h"

#include "csection.h"

FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormGPS)
{
    ui->setupUi(this);
}

FormGPS::~FormGPS()
{
    delete ui;
}
