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
#include <QQmlApplicationEngine>
#include <QSerialPort>

#include "common.h"

#include "vecfix2fix.h"
#include "vec2.h"
#include "vec3.h"
#include "cflag.h"
#include "cmodulecomm.h"
#include "ccamera.h"
#include "btnenum.h"

#include "cworldgrid.h"
#include "cnmea.h"
#include "cvehicle.h"
#include "ctool.h"
#include "cboundary.h"
#include "cabline.h"
#include "ctram.h"
#include "ccontour.h"
#include "cabcurve.h"
#include "cyouturn.h"
#include "cfielddata.h"
#include "csim.h"
#include "cahrs.h"
#include "crecordedpath.h"
#include "cguidance.h"
#include "cheadline.h"
#include "cpgn.h"
#include "aogiface_property.h"

//forward declare classes referred to below, to break circular
//references in the code
class QOpenGLShaderProgram;
class AOGRendererInSG;
class QQuickCloseEvent;
class QVector3D;

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
    QTimer timerSim;

    /***************************
     * Qt and QML GUI elements *
     ***************************/
    //QQuickView *qmlview;
    QWidget *qmlcontainer;
    AOGRendererInSG *openGLControl;
    //Left Menu & children
    QObject *btnAcres;
    QObject *btnSettings;
    QObject *btnTools;
    QObject *btnFieldMenu;
    QObject *btnAgIO;
    QObject *btnautoSteerConf;
    QObject *btnAutoSteerMode;

    QObject *btnTiltDown;
    QObject *btnTiltUp;
    QObject *btnZoomIn;
    QObject *btnZoomOut;

    //right menu & children
    QObject *btnContour;
    QObject *btnCurve;
    QObject *btnABLine;
    QObject *btnABLineCycle;
    QObject *btnABLineCycleBk;
    QObject *btnManualOffOn;
    QObject *btnSectionOffAutoOn;
    QObject *btnAutoSteer;

    QObject *btnContourPriority;
    //other
    QObject *btnFlag;

    //flag context menu and buttons
    QObject *contextFlag;
    QObject *btnDeleteFlag;
    QObject *btnDeleteAllFlags;

    //section buttons
    QObject *sectionButton[MAXSECTIONS-1]; //zero based array

    QObject *txtDistanceOffABLine;

    //offscreen GL objects:
    QSurfaceFormat backSurfaceFormat;
    QOpenGLContext backOpenGLContext;
    QOffscreenSurface backSurface;
    QOpenGLFramebufferObject *backFBO = 0;

    /*******************
     * from FormGPS.cs *
     *******************/
    //The base directory where AgOpenGPS will be stored and fields and vehicles branch from
    QString baseDirectory;

    //current directory of vehicle
    QString vehiclesDirectory, vehicleFileName = "";

    //current directory of tools
    QString toolsDirectory, toolFileName = "";

    //current directory of Environments
    QString envDirectory, envFileName = "";

    //current fields and field directory
    QString fieldsDirectory, currentFieldDirectory, displayFieldName;


    bool leftMouseDownOnOpenGL; //mousedown event in opengl window
    int flagNumberPicked = 0;

    //bool for whether or not a job is active
    bool /*isJobStarted = false,*/ isAreaOnRight = true /*, isAutoSteerBtnOn = false*/;

    //this bool actually lives in the QML aog object.
    AOGIFace_Property<bool> isJobStarted = AOGIFace_Property<bool>("isJobStarted");
    AOGIFace_Property<bool> isAutoSteerBtnOn = AOGIFace_Property<bool>("isAutoSteerBtnOn");

    //if we are saving a file
    bool isSavingFile = false, isLogElevation = false;

    //the currentversion of software
    QString currentVersionStr, inoVersionStr;

    int inoVersionInt;

    //create instance of a stopwatch for timing of frames and NMEA hz determination
    QElapsedTimer swFrame;

    double secondsSinceStart;

    //Time to do fix position update and draw routine
    double frameTime = 0;

    //Time to do fix position update and draw routine
    double gpsHz = 10;

    bool isStanleyUsed = false;

    int pbarSteer, pbarRelay, pbarUDP;

    double nudNumber = 0;

    double m2InchOrCm, inchOrCm2m, m2FtOrM, ftOrMtoM, cm2CmOrIn, inOrCm2Cm;
    QString unitsFtM, unitsInCm;

    //used by filePicker Form to return picked file and directory
    //QString filePickerFileAndDirectory;

    //the position of the GPS Data window within the FormGPS window
    //int GPSDataWindowLeft = 76, GPSDataWindowTopOffset = 220;

    //the autoManual drive button. Assume in Auto
    bool isInAutoDrive = true;

    //isGPSData form up
    bool isGPSSentencesOn = false, isKeepOffsetsOn = false;


private:
    //For field saving in background
    int minuteCounter = 1;

    int tenMinuteCounter = 1;

    //used to update the screen status bar etc
    int displayUpdateHalfSecondCounter = 0, displayUpdateOneSecondCounter = 0, displayUpdateOneFifthCounter = 0, displayUpdateThreeSecondCounter = 0;
    int tenSecondCounter = 0, tenSeconds = 0;
    int threeSecondCounter = 0, threeSeconds = 0;
    int oneSecondCounter = 0, oneSecond = 0;
    int oneHalfSecondCounter = 0, oneHalfSecond = 0;
    int oneFifthSecondCounter = 0, oneFifthSecond = 0;


     /*******************
     * GUI.Designer.cs *
     *******************/
public:
    //ABLines directory
    QString ablinesdirectory;

    //colors for sections and field background
    uchar flagColor = 0;

    //how many cm off line per big pixel
    int lightbarCmPerPixel = 2;

    //polygon mode for section drawing
    bool isDrawPolygons = false;

    QColor frameDayColor;
    QColor frameNightColor;
    QColor sectionColorDay;
    QColor fieldColorDay;
    QColor fieldColorNight;

    QColor textColorDay;
    QColor textColorNight;

    QColor vehicleColor;
    double vehicleOpacity;
    uchar vehicleOpacityByte;
    bool isVehicleImage;

    //Is it in 2D or 3D, metric or imperial, display lightbar, display grid etc
    bool isMetric = true, isLightbarOn = true, isGridOn, isFullScreen;
    bool isUTurnAlwaysOn, isCompassOn, isSpeedoOn, isSideGuideLines = true;
    bool isPureDisplayOn = true, isSkyOn = true, isRollMeterOn = false, isTextureOn = true;
    bool isDay = true, isDayTime = true, isBrightnessOn = true;
    bool isKeyboardOn = true, isAutoStartAgIO = true, isSvennArrowOn = true;

    bool isUTurnOn = true, isLateralOn = true;

    //sunrise, sunset

    bool isFlashOnOff = false;
    //makes nav panel disappear after 6 seconds
    int navPanelCounter = 0;

    uint sentenceCounter = 0;


    //master Manual and Auto, 3 states possible
    //btnStates manualBtnState = btnStates::Off;
    //btnStates autoBtnState = btnStates::Off;
    AOGIFace_Property<btnStates> manualBtnState = AOGIFace_Property<btnStates>("manualBtnState");
    AOGIFace_Property<btnStates> autoBtnState = AOGIFace_Property<btnStates>("manualBtnState");

    void FixPanelsAndMenus();

private:
public:
    //for animated submenu
    //bool isMenuHid = true;

    //Zoom variables
    double gridZoom;
    double zoomValue = 10;
    double triangleResolution = 1.0;
    double previousZoom = 25;

    // Storage For Our Tractor, implement, background etc Textures
    //Texture particleTexture;

    QElapsedTimer stopwatch; //general stopwatch for debugging purposes.
    //readonly Stopwatch swFrame = new Stopwatch();



    //Time to do fix position update and draw routine
    double HzTime = 5;

    QVector<CPatches> triStrip = QVector<CPatches>( { CPatches() } );


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

    CGuidance gyd;

    CTram tram;

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

    CSim sim;
    CAHRS ahrs;
    CRecordedPath recPath;
    CFieldData fd;

    CHeadLine hdl;

    /*
     * PGNs *
     */
    CPGN_FE p_254;
    CPGN_FC p_252;
    CPGN_FB p_251;
    CPGN_EF p_239;
    CPGN_EE p_238;
    CPGN_EC p_236;
    CPGN_EB p_235;
    CPGN_E5 p_229;

    /* GUI synchronization lock */
    QReadWriteLock lock;
    bool newframe = false;

    bool bootstrap_field = false;
   /************************
     * Controls.Designer.cs *
     ************************/
public:
    bool isTT;
    bool isABCyled = false;

    void GetHeadland();
    void CloseTopMosts();
    void SnapToPivot();
    void getAB();
    void FixTramModeButton();

    //other things will be in slots

    /*************************
     *  Position.designer.cs *
     *************************/
public:
    //very first fix to setup grid etc
    bool isFirstFixPositionSet = false, isGPSPositionInitialized = false, isFirstHeadingSet = false;
    bool /*isReverse = false (CVehicle),*/ isSteerInReverse = true, isSuperSlow = false, isAutoSnaptoPivot = false;
    double startGPSHeading = 0;

    //string to record fixes for elevation maps
    QByteArray sbFix;

    // autosteer variables for sending serial moved to CVehicle
    //short guidanceLineDistanceOff, guidanceLineSteerAngle; --> CVehicle
    double avGuidanceSteerAngle;

    short errorAngVel;
    double setAngVel, actAngVel;
    bool isConstantContourOn;

    //guidance line look ahead
    double guidanceLookAheadTime = 2;
    Vec2 guidanceLookPos;

    //for heading or Atan2 as camera
    QString headingFromSource, headingFromSourceBak;

    /* moved to CVehicle:
    Vec3 pivotAxlePos;
    Vec3 steerAxlePos;
    Vec3 toolPos;
    Vec3 tankPos;
    Vec3 hitchPos;
    */

    //history
    Vec2 prevFix;
    Vec2 prevDistFix;
    Vec2 lastReverseFix;

    //headings
    double smoothCamHeading = 0, gpsHeading = 10.0, prevGPSHeading = 0.0;

    //storage for the cos and sin of heading
    //moved to vehicle
    //double cosSectionHeading = 1.0, sinSectionHeading = 0.0;

    //how far travelled since last section was added, section points
    double sectionTriggerDistance = 0, contourTriggerDistance = 0, sectionTriggerStepDistance = 0;
    Vec2 prevSectionPos;
    Vec2 prevContourPos;
    int patchCounter = 0;

    Vec2 prevBoundaryPos;

    //Everything is so wonky at the start
    int startCounter = 0;

    //individual points for the flags in a list
    QVector<CFlag> flagPts;
    bool flagsBufferCurrent = false;

    //tally counters for display
    //public double totalSquareMetersWorked = 0, totalUserSquareMeters = 0, userSquareMetersAlarm = 0;


    double /*avgSpeed --> CVehicle,*/ previousSpeed;//for average speed
    double crossTrackError; //for average cross track error

    //youturn
    double distancePivotToTurnLine = -2222;
    double distanceToolToTurnLine = -2222;

    //the value to fill in you turn progress bar
    int youTurnProgressBar = 0;

    //IMU
    double rollCorrectionDistance = 0;
    double imuGPS_Offset, imuCorrected;

    //step position - slow speed spinner killer
    int currentStepFix = 0;
    int totalFixSteps = 10;
    VecFix2Fix stepFixPts[10];
    double distanceCurrentStepFix = 0, distanceCurrentStepFixDisplay = 0, minHeadingStepDist = 1, startSpeed = 0.5;
    double fixToFixHeadingDistance = 0, gpsMinimumStepDistance = 0.05;

    bool isChangingDirection, isReverseWithIMU;

    double nowHz = 0, filteredDelta = 0, delta = 0;

    bool isRTK, isRTK_KillAutosteer;

    double headlandDistanceDelta = 0, boundaryDistanceDelta = 0;

    Vec2 lastGPS;

    double uncorrectedEastingGraph = 0;
    double correctionDistanceGraph = 0;

    double frameTimeRough = 3;
    double timeSliceOfLastFix = 0;

    bool isMaxAngularVelocity = false;

    int minSteerSpeedTimer = 0;

    void UpdateFixPosition(); //process a new position
    void TheRest();
    void CalculatePositionHeading(); // compute all headings and fixes
    void AddBoundaryPoint();
    void AddContourPoints();
    void AddSectionOrPathPoints();
    void CalculateSectionLookAhead(double northing, double easting, double cosHeading, double sinHeading);
    void InitializeFirstFewGPSPositions();

    /************************
     * SaveOpen.Designer.cs *
     ************************/

    //moved to CTool
    //list of the list of patch data individual triangles for field sections
    //QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    //moved to CContour.
    //list of the list of patch data individual triangles for contour tracking
    QVector<QSharedPointer<QVector<Vec3>>> contourSaveList;

    void FileSaveHeadLines();
    void FileLoadHeadLines();
    void FileSaveCurveLines();
    void FileLoadCurveLines();
    void FileSaveABLines();
    void FileLoadABLines();
    bool FileOpenField(QString fieldDir);
    void FileCreateField();
    void FileCreateElevation();
    void FileSaveSections();
    void FileCreateSections();
    void FileCreateFlags();
    void FileCreateContour();
    void FileSaveContour();
    void FileCreateBoundary();
    void FileSaveBoundary();
    void FileSaveTram();
    void FileSaveBackPic();
    void FileCreateRecPath();
    void FileSaveHeadland();
    void FileSaveRecPath();
    void FileLoadRecPath();
    void FileSaveFlags();
    void FileSaveNMEA();
    void FileSaveElevation();
    void FileSaveSingleFlagKML2(int flagNumber);
    void FileSaveSingleFlagKML(int flagNumber);
    void FileMakeKMLFromCurrentPosition(double lat, double lon);
    void ExportFieldAs_KML();
    void FileUpdateAllFieldsKML();
    QString GetBoundaryPointsLatLon(int bndNum);
    void ExportFieldAs_ISOXMLv3();
    void ExportFieldAs_ISOXMLv4();


    /************************
     * formgps_sections.cpp *
     ************************/
    //void SectionSetPosition();
    //void SectionCalcWidths();
    //void SectionCalcMulti();
    void BuildMachineByte();
    void DoRemoteSwitches();


    /************************
     * formgps_settimgs.cpp *
     ************************/

    void loadSettings();

    /**********************
     * OpenGL.Designer.cs *
     **********************/
    //extracted Near, Far, Right, Left clipping planes of frustum
    double frustum[24];

    double fovy = 0.7;
    double camDistanceFactor = -2;
    int mouseX = 0, mouseY = 0;
    double mouseEasting = 0, mouseNorthing = 0;
    int lastWidth=-1, lastHeight=-1;
    double maxFieldX, maxFieldY, minFieldX, minFieldY, fieldCenterX, fieldCenterY, maxFieldDistance;
    double offX = 0.0, offY = 0.0;

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

    /***********************
     * formgps_udpcomm.cpp *
     ***********************/
private:
    QUdpSocket *udpSocket = NULL;

public:
    QElapsedTimer udpWatch;
    int udpWatchLimit = 70;
    int udpWatchCounts = 0;

    bool isUDPServerOn = false;

    void StartLoopbackServer();
    void stopUDPServer();

    void SendPgnToLoop(QByteArray byteData);
    void DisableSim();
    //void ReceiveFromAgIO(); // in slots below

    /******************
     * formgps_ui.cpp *
     ******************/
    //or should be under formgps_settings.cpp?

   /**********************
     * OpenGL.Designer.cs *
     **********************/
    ulong number = 0, lastNumber = 0;

    bool isHeadlandClose = false;

    int steerModuleConnectedCounter = 0;
    double avgPivDistance=0, lightbarDistance=0;
    QString strHeading;
    int lenth = 4;

    void DrawManUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    void DrawUTurnBtn(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    void MakeFlagMark(QOpenGLFunctions *gl);
    void DrawFlags(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    void DrawSteerCircle(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection);

    void DrawLightBar(QOpenGLFunctions *gl, QMatrix4x4 mvp, double offlineDistance);
    void DrawLightBarText(QOpenGLFunctions *gl, QMatrix4x4 mvp);
    //void drawRollBar(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection);
    void DrawSky(QOpenGLFunctions *gl, QMatrix4x4 mvp, int width, int height);
    void DrawCompassText(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width, double Height);
    void DrawCompass(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width);
    void DrawReverse(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width, double Height);
    void drawSpeedo(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, double Width, double Height);
    void DrawLiftIndicator(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, int Width, int Height);
    void DrawLostRTK(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width);
    void DrawAge(QOpenGLFunctions *gl, QMatrix4x4 mvp, double Width);
    void CalcFrustum(const QMatrix4x4 &mvp);
    void calculateMinMax();

    QVector3D mouseClickToField(int mouseX, int mouseY);
    QVector3D mouseClickToPan(int mouseX, int mouseY);

    void SetZoom();
    void loadGLTextures();

private:
    void setupGui();


    /**************
     * FormGPS.cs *
     **************/
public:
    QString speedMPH();
    QString speedKPH();

    void jobNew();
    void JobClose();

    /***********************
     * FormGPS.Designer.cs *
     ***********************/
    void manualAllBtnsUpdate();
    void manualBtnUpdate(int sectNumber);
    void lineUpManualBtns();

    // formgps_ui
    bool closeAllMenus();


    /******************************
     * formgps_classcallbacks.cpp *
     ******************************/
    void connect_classes();


    /****************
     * form_sim.cpp *
     ****************/
    void simConnectSlots();

    /**************************
     * UI/Qt object callbacks *
     **************************/

public slots:
    /*******************
     * from FormGPS.cs *
     *******************/
    void tmrWatchdog_timeout();
    void processSectionLookahead(); //called when section lookahead GL stuff is rendered

    void onGLControl_clicked(const QVariant &event);
    void onGLControl_dragged(int startX, int startY, int mouseX, int mouseY);

    void TimedMessageBox(int timeout, QString s1, QString s2);

    void update_ABlines_in_qml();
    void update_current_ABline_from_qml();
    void add_new_ABline(QString name, double easting, double northing,double heading);
    void start_newABLine(bool start_or_cancel, double easting, double northing, double heading);

    //left column
    void onBtnAcres_clicked();
    void onBtnSettings_clicked();
    void onBtnAgIO_clicked();
    void onBtnSteerConfig_clicked();
    void onBtnSteerMode_clicked();
    //right column
    void onBtnContour_clicked();
    void onBtnABCurve_clicked();
    void onBtnABLine_clicked();
    void onBtnToggleAB_clicked();
    void onBtnToggleABBack_clicked();
    void onBtnAutoYouTurn_clicked();
    void onBtnAutoSteer_clicked();
    void onBtnContourPriority_clicked();
    //bottom row
    void onBtnResetTool_clicked();
    void onBtnHeadland_clicked();
    void onBtnHydLift_clicked();
    void onBtnFlag_clicked();
    void onBtnTramlines_clicked();
    void onBtnSectionColor_clicked();
    void onBtnLinePicker_clicked();
    void onBtnSnapToPivot_clicked();
    //don't need ablineedit
    void onBtnYouSkip_clicked();


    void onBtnTiltDown_clicked();
    void onBtnTiltUp_clicked();

    void onBtnZoomIn_clicked();
    void onBtnZoomOut_clicked();

    void onBtnRedFlag_clicked();
    void onBtnGreenFlag_clicked();
    void onBtnYellowFlag_clicked();
    void onBtnDeleteFlag_clicked();
    void onBtnDeleteAllFlags_clicked();

    void swapDirection();
    void turnOffBoundAlarm();

    void onBtnManUTurnLeft_clicked();
    void onBtnManUTurnRight_clicked();

    /***************************
     * from OpenGL.Designer.cs *
     ***************************/
    void oglMain_Paint();
    void openGLControl_Initialized();
    void openGLControl_Shutdown();
    //void openGLControl_Resize();

    void oglBack_Paint();
    void openGLControlBack_Initialized();

    /***
     * UDPCOMM.Designer.cs
     * formgps_udpcomm.cpp
     ***/
    void ReceiveFromAgIO(); // in slots below


    /*
     * From Position.Designer.cs
     */
    //moved to public slots
    //void processSectionLookahead(); //called when section lookahead GL stuff is rendered

    /*******************
     * simulator       *
     * formgps_sim.cpp *
     *******************/
    void onSimNewPosition(double vtgSpeed,
                     double headingTrue,
                     double latitude,
                     double longitude, double hdop,
                     double altitude,
                     double satellitesTracked);

    void onSimNewSteerAngle(double steerAngleAve);

    void onSimTimerTimeout();

    /*
     * misc
     */
    void fileSaveEverythingBeforeClosingField(QQuickCloseEvent *event);

    /* formgps_classcallbacks.cpp */
    void onStopAutoSteer(); //cancel autosteer and ensure button state
    void onSectionMasterAutoOff();
    void onSectionMasterManualOff();
    void onStoppedDriving();

};

#endif // FORMGPS_H
