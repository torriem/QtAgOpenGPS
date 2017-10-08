#include "formgps.h"
#include "ui_formgps.h"
#include "cworldgrid.h"
#include "cnmea.h"
#include "csection.h"
#include "cabline.h"
#include "ccontour.h"
#include "cvehicle.h"
#include "mainoverlay.h"


FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormGPS)
{
    setupGui();

    /* initialize child objects */
    /* these objects need pointers to this form
     * to retrieve information. they are tightly
     * coupled!  So we have to create them here, since
     * the header file has only incomplete types.
     */
    worldGrid = new CWorldGrid(this);
    pn = new CNMEA(this);
    section = new CSection[MAXSECTIONS];
    for (int i = 0; i < MAXSECTIONS; i++)
        section->set_mainform(this);

    ABLine = new CABLine(this);
    ct = new CContour(this);
    vehicle = new CVehicle(this);
}

FormGPS::~FormGPS()
{
    /* clean up our dynamically-allocated
     * objects.
     */
    delete ui;
    delete worldGrid;
    delete pn;
    delete[] section;
    delete ABLine;
    delete ct;
    delete vehicle;

}



