#include "mainoverlay.h"
#include "ui_mainoverlay.h"

MainOverlay::MainOverlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainOverlay)
{
    ui->setupUi(this);
}

MainOverlay::~MainOverlay()
{
    delete ui;
}
