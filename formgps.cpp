#include "formgps.h"
#include "ui_formgps.h"
#include "cworldgrid.h"
#include "cnmea.h"
#include "csection.h"
#include "cabline.h"
#include "ccontour.h"
#include "cboundary.h"
#include "cvehicle.h"
#include "mainoverlay.h"
#include "aogsettings.h"

FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormGPS)
{
    setupGui();
    AOGSettings s;

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
    boundary = new CBoundary(this);

    isUDPServerOn = s.value("port/udp_on", true).toBool();


    /* test data to see if drawing routines are working. */
    isGPSPositionInitialized = true;
    pivotAxlePos.easting = 5005;
    pivotAxlePos.northing = 5005;
    fixEasting = 5005;
    fixNorthing = 5000;
    fixHeadingSection = 0;
    fixHeading = 0;
    fixHeadingCam = 0;
    fixHeadingSection = 0;
    worldGrid->createWorldGrid(5005,5005);
    isGridOn = true;

    section[0].positionLeft = -8.0;
    section[0].positionRight = -3.0;
    section[1].positionLeft = -3.0;
    section[1].positionRight = 3.0;
    section[2].positionLeft = 3.0;
    section[2].positionRight = 8.0;

    if (isUDPServerOn) startUDPServer();

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



