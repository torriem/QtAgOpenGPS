#ifndef FORMGPS_H
#define FORMGPS_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QtQuick/QQuickItem>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
//#include <QQuickView>
#include <QQmlApplicationEngine>

#include "common.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "cflag.h"
#include "cmodulecomm.h"
#include "ccamera.h"
#include "btnenum.h"
#include "aogsettings.h"

#include "cworldgrid.h"
#include "cnmea.h"
#include "cvehicle.h"
#include "ctool.h"
#include "cboundary.h"
#include "cturn.h"
#include "cabline.h"
#include "ctram.h"
#include "cmazegrid.h"
#include "ccontour.h"
#include "cabcurve.h"
#include "cyouturn.h"
#include "chead.h"
#include "csequence.h"
#include "cfielddata.h"
#include "csim.h"
#include "cahrs.h"
#include "crecordedpath.h"
#include "cgeofence.h"

//forward declare classes referred to below, to break circular
//references in the code
class TopLineDisplay;

class QOpenGLShaderProgram;
class AOGRendererInSG;

class FormGPS : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit FormGPS(QWidget *parent = 0);
    ~FormGPS();

     /***********************************************
     * Qt-specific things we need to keep track of *
     ***********************************************/
    QLocale locale;
    QObject *qml_root;
    QSignalMapper *sectionButtonsSignalMapper;
    QTimer *tmrWatchdog;
    QTimer simTimer;

    /***************************
     * Qt and QML GUI elements *
     ***************************/
    //QQuickView *qmlview;
    QWidget *qmlcontainer;
    AOGRendererInSG *openGLControl;
    QObject *btnMinMaxZoom;
    QObject *btnPerimeter;
    QObject *btnAutoSteer;
    QObject *btnFlag;
    QObject *btnABLine;
    QObject *btnContour;
    QObject *btnManualOffOn;
    QObject *btnSectionOffAutoOn;

    QObject *btnTiltDown;
    QObject *btnTiltUp;

    QObject *btnZoomIn;
    QObject *btnZoomOut;

    //menu button and icon palette -- do we need to keep all of these?
    QObject *btnMenuDrawer;
    QObject *btnSnap;
    QObject *btnTripOdometer;
    QObject *btnGPSData;
    QObject *btnSettings;
    QObject *btnJob;
    QObject *btnBoundaryMenu;
    QObject *btnComm;
    QObject *btnUnits;
    QObject *btnFileExplorer;
    QObject *btnAutoSteerConfig;

    //area context menu
    QObject *contextArea;

    //flag context menu and buttons
    QObject *contextFlag;
    QObject *btnDeleteFlag;
    QObject *btnDeleteAllFlags;

    //section buttons
    QObject *sectionButton[MAXSECTIONS-1]; //zero based array

    QObject *txtDistanceOffABLine;

    AOGSettings settings;

    //offscreen GL objects:
    QSurfaceFormat backSurfaceFormat;
    QOpenGLContext backOpenGLContext;
    QOffscreenSurface backSurface;
    QOpenGLFramebufferObject *backFBO = 0;

    /*******************
     * from FormGPS.cs *
     *******************/
    //current directory of field;
    QString currentFieldDirectory = "";
    QString workingDirectory = "";
    QString vehicleFileName = "";
    QString toolFileName = "";

    //colors for sections and field background
    //uchar redSections, grnSections, bluSections;
    //keep these in QSettings
    //QColor sectionColor;
    //QColor fieldColor;
    //uchar redField, grnField, bluField;

    //polygon mode for section drawing
    bool isDrawPolygons = false;

    //for animated submenu
    bool isMenuHid = true;

    //Flag stuff
    uchar flagColor = 0;
    bool leftMouseDownOnOpenGL = false; //mousedown event in opengl window
    int flagNumberPicked = 0;

    //Is it in 2D or 3D, metric, or imperial, display lightbar, display grid
    //following are in settings now:
    //isGridOn, isMetric
    bool isIn3D = true, isLightbarOn = true, isSideGuideLines = true;
    //bool isPureOn = true; //refer to QtSettings instead

    //bool for whether or not a job is active
    bool isJobStarted = false, isAreaOnRight = true, isAutoSteerBtnOn = false;

    //master Manual and Auto, 3 states possible
    btnStates manualBtnState = btnStates::Off;
    btnStates autoBtnState = btnStates::Off;

    //if we are saving a file
    bool isSavingFile = false, isLogNMEA = false;

    //Zoom variables
    double gridZoom;
    double zoomValue = 10;
    double triangleResolution = 1.0;
    double previousZoom = 25;

    // Storage For Our Tractor, implement, background etc Textures
    //Texture particleTexture;

    //create instance of a stopwatch for timing of frames and NMEA hz determination
    QElapsedTimer swFrame;
    QElapsedTimer stopwatch; //general stopwatch for debugging purposes.
    //readonly Stopwatch swFrame = new Stopwatch();

    //Time to do fix position update and draw routine
    double frameTime = 0;

    //Time to do fix position update and draw routine
    double HzTime = 5;

private:
    //for the NTRIP CLient counting
    int ntripCounter = 10;

    //used to update the screen status bar etc
     int displayUpdateHalfSecondCounter = 0, displayUpdateOneSecondCounter = 0, displayUpdateOneFifthCounter = 0, displayUpdateThreeSecondCounter = 0;

     int threeSecondCounter = 0, threeSeconds = 0;
     int oneSecondCounter = 0, oneSecond = 0;
     int oneHalfSecondCounter = 0, oneHalfSecond = 0;
     int oneFifthSecondCounter = 0, oneFifthSecond = 0;
     int minuteCounter = 1;
public:
     int pbarSteer, pbarRelay, pbarUDP;
     double nudNumber = 0;


    //used to update the screen status bar etc
    int statusUpdateCounter = 1;

    //create the scene camera
    CCamera camera;

    //create world grid
    //QScopedPointer <CWorldGrid> worldGrid;
    CWorldGrid worldGrid;

    //Parsing object of NMEA sentences
    //QScopedPointer<CNMEA> pn;
    CNMEA pn;

    //ABLine Instance
    //QScopedPointer<CABLine> ABLine;
    CABLine ABLine;
    CABCurve curve;


    CTram tram;

    CMazeGrid mazeGrid;

    //Contour mode Instance
    //QScopedPointer<CContour> ct;
    CContour ct;
    CYouTurn yt;

    CVehicle vehicle;
    CTool tool;

    //module communication object
    CModuleComm mc;

    //boundary instance
    CBoundary bnd;

    CTurn turn;
    CHead hd;
    CSequence seq;
    CSim sim;
    CAHRS ahrs;
    CRecordedPath recPath;
    CFieldData fd;
    CGeoFence gf;

    bool bootstrap_field = false;


    /*************************
     *  Position.designer.cs *
     *************************/
    double toLatitude;
    double toLongitude;

    //very first fix to setup grid etc
    bool isFirstFixPositionSet = false, isGPSPositionInitialized = false;

    // autosteer variables for sending serial moved to CVehicle
    //short int guidanceLineDistanceOff, guidanceLineSteerAngle;

    //how many fix updates per sec
    int fixUpdateHz = 5;
    double fixUpdateTime = 0.2;

    QString headingFromSource;
    QString headingFromSourceBak;

    /*
    double fixHeadingSection = 0.0, fixHeadingTank = 0.0;
    Vec2 pivotAxlePos;
    Vec2 toolPos;
    Vec2 tankPos;
    Vec2 hitchPos;*/

    Vec2 prevFix;

    //headings
    double gpsHeading = 0.0, prevGPSHeading = 0.0;


    //a distance between previous and current fix
    double distance = 0.0;
    double treeSpacingCounter = 0.0;
    int treeTrigger = 0;

    //how far travelled since last section was added, section points
    double sectionTriggerDistance = 0, sectionTriggerStepDistance = 0;
    Vec2 prevSectionPos = Vec2(0, 0);

    Vec2 prevBoundaryPos = Vec2(0, 0);

    QByteArray sbFix;


    //headings
    //double fixHeading = 0.0;
    double fixHeadingCam = 0.0;

    //storage for the cos and sin of heading
    //double cosSectionHeading = 1.0, sinSectionHeading = 0.0;

    //are we still getting valid data from GPS, resets to 0 in NMEA RMC block, watchdog
    int recvCounter = 20;

    //Everything is so wonky at the start
    int startCounter = 0;

    //individual points for the flags in a list
    QVector<CFlag> flagPts;
    bool flagsBufferCurrent = false;



    //used to determine NMEA sentence frequency
    int timerPn = 1;
    double et = 0, hzTime = 0;

    double crossTrackError; //for average cross track error

    //youturn
    double distancePivotToTurnLine = -2222;
    double distanceToolToTurnLine = -2222;



    //IMU
    double rollDistance = 0;
    double roll = 0; //, pitch = 0, angVel = 0;
    double avgRoll = 0; //, avgPitch = 0, avgAngVel = 0;
    double rollCorrectionDistance = 0;
    double gyroCorrection, gyroCorrected;

    double rollUsed;
    double headlandDistanceDelta = 0, boundaryDistanceDelta = 0;


    //cautosteer
    //flag for free drive window to control autosteer
    bool isInFreeDriveMode = false;
    int driveFreeSteerAngle = 0;




    int times;

    int totalFixSteps = 10, currentStepFix = 0;
    Vec3 vHold;
    Vec3 stepFixPts[60];
    //int fence = 0x0ff;
    double distanceCurrentStepFix = 0, fixStepDist=0; //, minFixStepDist = 0;
    bool isFixHolding = false, isFixHoldLoaded = false;

    double rollZero = 0, pitchZero = 0;
    double rollAngle = 0, pitchAngle = 0;

    //step distances and positions for boundary, 6 meters before next point
    double boundaryTriggerDistance = 6.0;

    bool isBoundAlarming = false;

    void updateFixPosition(); //process a new position
    void calculatePositionHeading(); // compute all headings and fixes
    void addBoundaryPoint();
    void addSectionOrContourPathPoints();
    void calculateSectionLookAhead(double northing, double easting, double cosHeading, double sinHeading);
    void initializeFirstFewGPSPositions();
    bool isInsideGeoFence();

    /************************
     * SaveOpen.Designer.cs *
     ************************/

    //moved to CTool
    //list of the list of patch data individual triangles for field sections
    //QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    //moved to CContour.
    //list of the list of patch data individual triangles for contour tracking
    QVector<QSharedPointer<QVector<Vec3>>> contourSaveList;

    void fileSaveCurveLines();
    void fileLoadCurveLines();
    void fileSaveABLines();
    void fileLoadABLines();
    void fileSaveVehicle(QString filename);
    bool fileOpenVehicle(QString filename);
    void fileSaveTool(QString filename);
    bool fileOpenTool(QString filename);
    void fileSaveEnvironment(QString filename);
    bool fileOpenEnvironment(QString filename);
    bool fileOpenField(QString fieldDir);
    void fileCreateField();
    void fileCreateElevation();
    void fileSaveSections();
    void fileCreateSections();
    void fileCreateFlags();
    void fileCreateContour();
    void fileSaveContour();
    void fileSaveBoundary();
    void fileCreateRecPath();
    void fileSaveHeadland();
    void fileSaveRecPath();
    void fileSaveFlags();
    void fileSaveNMEA();
    void fileSaveElevation();
    void fileSaveSingleFlagKML2(int flagNumber);
    void fileSaveSingleFlagKML(int flagNumber);
    void fileMakeKMLFromCurrentPosition(double lat, double lon);
    void fileSaveFieldKML();

    /**********************
     * OpenGL.Designer.cs *
     **********************/
    //extracted Near, Far, Right, Left clipping planes of frustum
    double frustum[24];

    double fovy = 0.7;
    double camDistanceFactor = -2;
    int mouseX = 0, mouseY = 0;
    int lastWidth=-1, lastHeight=-1;
    double maxFieldX, maxFieldY, minFieldX, minFieldY, fieldCenterX, fieldCenterY, maxFieldDistance;
    double offX = 0.0, offY = 0.0;

    bool isDay;

    //data buffer for pixels read from off screen buffer
    //uchar grnPixels[80001];
    LookAheadPixels grnPixels[80001];
    QImage grnPix;

    /*
    QOpenGLShaderProgram *simpleColorShader = 0;
    QOpenGLShaderProgram *texShader = 0;
    QOpenGLShaderProgram *interpColorShader = 0;
    */
    QOpenGLBuffer skyBuffer;
    QOpenGLBuffer flagsBuffer;

    uint sentenceCounter = 0;


    /***********************
     * UDPComm.designer.cs *
     ***********************/
private:
    QUdpSocket *udpSocket = NULL;

public:
    bool isUDPServerOn = false;

    void startUDPServer();
    void stopUDPServer();

    void sendUDPMessage(uchar *msg);

   /**********************
     * OpenGL.Designer.cs *
     **********************/
    /*
    //Simple wrapper to draw primitives using lists of Vec3 or QVector3Ds
    //with a single color.
    void glDrawArraysColor(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                           GLenum operation, QColor &color,
                           QOpenGLBuffer &vertexBuffer, GLenum glType,
                           int count,
                           float pointSize=1.0f);
    //Simple wrapper to draw primitives using lists of vec3s or QVector3Ds
    //with a color per vertex. Buffer format is 7 floats per vertice:
    //x,y,z,r,g,b,a
    void glDrawArraysColors(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                           GLenum operation,
                           QOpenGLBuffer &vertexBuffer, GLenum glType,
                           int count,
                           float pointSize=1.0f);

    //Buffer format is 5 floats per vertice:
    //x,y,z,texX,texY
    void glDrawArraysTexture(QOpenGLFunctions *gl, QMatrix4x4 mvp,
                             GLenum operation,
                             QOpenGLBuffer &vertexBuffer, GLenum glType,
                             int count);
    */
    void drawManUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    void drawUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    void makeFlagMark(QOpenGLFunctions *gl);
    void drawFlags(QOpenGLFunctions *gl, QMatrix4x4 mvp);

    void drawLightBar(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width, double Height, double offlineDistance);
    void drawLightBarText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width, double Height);
    void drawRollBar(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection);
    void drawSky(QOpenGLFunctions *gl, QMatrix4x4 mvp, int width, int height);
    void drawCompassText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width);
    void drawCompass(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width);
    void drawSpeedo(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width, double Height);
    //void drawFieldText();
    void drawLiftIndicator(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, int Width, int Height);
    void calcFrustum(const QMatrix4x4 &mvp);
    void calculateMinMax();


    void setZoom();
    void loadGLTextures();

private:
    void setupGui();


    /**************
     * FormGPS.cs *
     **************/
public:
    QString speedMPH();
    QString speedKPH();
    void processSectionOnOffRequests(bool isMapping);
    bool scanForNMEA();

    void jobNew();
    void jobClose();

    /**************************
     * SerialComm.Designer.cs *
     **************************/
public:

    double actualSteerAngleDisp;
    void autoSteerDataOutToPort();
    void sendSteerSettingsOutAutoSteerPort();
    void sectionControlOutToPort();
    void sendOutUSBAutoSteerPort(uchar *data, int pgnSentenceLength);


    /***********************
     * FormGPS.Designer.cs *
     ***********************/
    void manualAllBtnsUpdate();
    void manualBtnUpdate(int sectNumber);
    void lineUpManualBtns();

    // formgps_ui
    bool closeAllMenus();


    /**************************
     * UI/Qt object callbacks *
     **************************/
public slots:
    /*******************
     * from FormGPS.cs *
     *******************/
    void onGLControl_clicked(const QVariant &event);

    void onBtnMinMaxZoom_clicked();
    void onBtnPerimeter_clicked();
    //void onBtnPerimeter_pressAndHeld();
    void onBtnAutoSteer_clicked();
    void onBtnFlag_clicked();
    void onBtnABLine_clicked();
    void onBtnContour_clicked();
    void onBtnManualOffOn_clicked();
    void onBtnSectionOffAutoOn_clicked();

    void onBtnTiltDown_clicked();
    void onBtnTiltUp_clicked();

    void onBtnZoomIn_clicked();
    void onBtnZoomOut_clicked();

    void onBtnSnap_clicked();
    void onBtnTripOdometer_clicked();
    void onBtnGPSData_clicked();
    void onBtnSettings_clicked();
    void onBtnJob_clicked();
    void onBtnBoundaryMenu_clicked();
    void onBtnComm_clicked();
    void onBtnUnits_clicked();
    void onBtnFileExplorer_clicked();
    void onBtnAutoSteerConfig_clicked();

    void onBtnAreaSide_clicked();

    //was btnSection#Man_Click in c#
    void onBtnSectionMan_clicked(int sectNumber);

    void onBtnRedFlag_clicked();
    void onBtnGreenFlag_clicked();
    void onBtnYellowFlag_clicked();
    void onBtnDeleteFlag_clicked();
    void onBtnDeleteAllFlags_clicked();

    void tmrWatchdog_timeout();

    void swapDirection();
    void turnOffBoundAlarm() {
        isBoundAlarming = false;
    }

    void onBtnManUTurnLeft_clicked();
    void onBtnManUTurnRight_clicked();

    /***************************
     * from OpenGL.Designer.cs *
     ***************************/
    void openGLControl_Draw();
    void openGLControl_Initialized();
    void openGLControl_Shutdown();
    //void openGLControl_Resize();

    void openGLControlBack_Draw();
    void openGLControlBack_Initialized();



    /*
     * from UDPComm.Designer.cs
     */

    void udpServerReadDatagrams();

    /*
     * From Position.Designer.cs
     */
    void processSectionLookahead(); //called when section lookahead GL stuff is rendered

    /*
     * simulator
     */
    void onSimNewPosition(QByteArray nmea_data);
    void onSimTimerTimeout();

    /*
     * misc
     */
    void onClearRecvCounter();

    /* CNMEA */
    void onHeadingSource(int);


    void fileSaveEverythingBeforeClosingField();
};

#endif // FORMGPS_H
