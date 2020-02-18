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
#include <QObject>

#include "common.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "cflag.h"
#include "cmodulecomm.h"
#include "cperimeter.h"
#include "ccamera.h"
#include "btnenum.h"
#include "aogsettings.h"
#include "csim.h"
#include "cnmea.h"
#include "cabline.h"
#include <QQuickView>


namespace Ui {
class FormGPS;
}
//forward declare classes referred to below, to break circular
//references in the code
class TopLineDisplay;

class CWorldGrid;
class CSection;
class CContour;
class CVehicle;
class CPerimeter;
class CBoundary;
class CRate;
class CYouTurn;

class QOpenGLShaderProgram;
class QQuickView;
class AOGRendererInSG;

class FormGPS : public QQuickView
{
    Q_OBJECT

public:
    explicit FormGPS(QWidget *parent = 0);
    ~FormGPS();

     /***********************************************
     * Qt-specific things we need to keep track of *
     ***********************************************/
    QLocale locale;
    QQuickItem *qml_root;
    QSignalMapper *sectionButtonsSignalMapper;
    QTimer *tmrWatchdog;

    /***************************
     * Qt and QML GUI elements *
     ***************************/
    QWidget *qmlcontainer;
    AOGRendererInSG *openGLControl;
    QObject *btnDisplayDrag;
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

    QObject *btnThreeD;
    QObject *btnTwoD;

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
    QString vehiclefileName = "";

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

    //flag for free drive window to control autosteer
    bool isInFreeDriveMode = false;

    //Flag stuff
    uchar flagColor = 0;
    bool leftMouseDownOnOpenGL = false; //mousedown event in opengl window
    int flagNumberPicked = 0;

    //Is it in 2D or 3D, metric, or imperial, display lightbar, display grid
    bool isIn3D = true, isMetric = true, isLightbarOn = true, isGridOn, isSideGuideLines = true;
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
    GLuint texture[3];
    QVector<QOpenGLTexture *> texture1;

    //create the scene camera
    CCamera camera;

    //create world grid
    //QScopedPointer <CWorldGrid> worldGrid;
    CWorldGrid *worldGrid;

    //create instance of a stopwatch for timing of frames and NMEA hz determination
    QElapsedTimer swFrame;
    QElapsedTimer stopwatch; //general stopwatch for debugging purposes.
    //readonly Stopwatch swFrame = new Stopwatch();

    //Time to do fix position update and draw routine
    double frameTime = 0;

    //For field saving in background
    int saveCounter = 1;

    //used to update the screen status bar etc
    int statusUpdateCounter = 1;

    //Parsing object of NMEA sentences
    //QScopedPointer<CNMEA> pn;
    CNMEA *pn;

    //create an array of sections, so far only 8 section + 1 fullWidth Section
    //QScopedArrayPointer<CSection> section;
    //CSection *section =NULL;

    //ABLine Instance
    //QScopedPointer<CABLine> ABLine;
    CABLine *ABLine;

    //Contour mode Instance
    //QScopedPointer<CContour> ct;
    CContour *ct;

    //Auto headland instance
    CYouTurn *yt;

    //Rate control object
    CRate *rc;

    //a brand new vehicle
    //QScopedPointer<CVehicle> vehicle;
    CVehicle *vehicle;

    //module communication object
    CModuleComm mc;

    //perimeter object for area calc
    CPerimeter periArea;

    //boundary instance
    CBoundary *boundary;



    /*************************
     *  Position.designer.cs *
     *************************/
    double toLatitude;
    double toLongitude;

    //very first fix to setup grid etc
    bool isFirstFixPositionSet = false, isGPSPositionInitialized = false;

    // autosteer variables for sending serial
    //short int guidanceLineDistanceOff, guidanceLineSteerAngle;

    //how many fix updates per sec
    int fixUpdateHz = 5;
    double fixUpdateTime = 0.2;

    //for heading or Atan2 as camera
    bool isAtanCam = true;

    /*
    //Current fix positions
    double fixEasting = 0.0;
    double fixNorthing = 3.0;
    double fixZ = 0.0;
    */

    Vec2 fix;
    /*
    double fixHeadingSection = 0.0, fixHeadingTank = 0.0;
    Vec2 pivotAxlePos;
    Vec2 toolPos;
    Vec2 tankPos;
    Vec2 hitchPos;*/

    Vec2 prevFix;

    //headings
    //double fixHeading = 0.0;
    double fixHeadingCam = 0.0;

    //storage for the cos and sin of heading
    //double cosSectionHeading = 1.0, sinSectionHeading = 0.0;

    //a distance between previous and current fix
    double distance = 0.0, userDistance = 0;


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
    //youturn
    double distancePivotToTurnLine = -2222;
    double distanceToolToTurnLine = -2222;

    //IMU
    double rollDistance = 0;
    double roll = 0; //, pitch = 0, angVel = 0;
    double avgRoll = 0; //, avgPitch = 0, avgAngVel = 0;

    int times;
    double avgTiltRoll[30];//for tilt
    int ringCounterTiltRoll = 0;
    //public double[] avgTiltPitch = new double[10];//for pitch
    //public int ringCounterTiltPitch = 0;
    //public double[] avgAngularVelocity = new double[30];//for angular velocity
    //public int ringCounterAngularVelocity = 0;

    double avgSpeed[10]; // for average speed
    double avgXTE[20]; // for average corss track error
    int ringCounter = 0, avgXTECntr, crossTrackError;

    int totalFixSteps = 10, currentStepFix = 0;
    Vec3 vHold;
    Vec3 stepFixPts[50];
    double distanceCurrentStepFix = 0, fixStepDist=0, minFixStepDist = 0;
    bool isFixHolding = false, isFixHoldLoaded = false;

    double rollZero = 0, pitchZero = 0;
    double rollAngle = 0, pitchAngle = 0;

    //step distances and positions for boundary, 6 meters before next point
    double boundaryTriggerDistance = 6.0;
    Vec2 prevBoundaryPos;

    void updateFixPosition(); //process a new position
    void calculatePositionHeading(); // compute all headings and fixes
    void addBoundaryPoint();
    void addSectionContourPathPoints();
    void calculateSectionLookAhead(double northing, double easting, double cosHeading, double sinHeading);
    void initializeFirstFewGPSPositions();





    /************************
     * SaveOpen.Designer.cs *
     ************************/
    //moved to CVehicle
    //list of the list of patch data individual triangles for field sections
    //QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    //moved to CContour.
    //list of the list of patch data individual triangles for contour tracking
    //QVector<QSharedPointer<QVector<Vec4>>> contourSaveList;

    /**********************
     * OpenGL.Designer.cs *
     **********************/
    //extracted Near, Far, Right, Left clipping planes of frustum
    double frustum[24];

    double fovy = 45;
    double camDistanceFactor = -2;
    int mouseX = 0, mouseY = 0;
    int lastWidth=-1, lastHeight=-1;

    //data buffer for pixels read from off screen buffer
    //uchar grnPixels[80001];
    LookAheadPixels lookaheadPixels[80001];
    QImage grnPix;

    /*
    QOpenGLShaderProgram *simpleColorShader = 0;
    QOpenGLShaderProgram *texShader = 0;
    QOpenGLShaderProgram *interpColorShader = 0;
    */
    QOpenGLBuffer skyBuffer;
    QOpenGLBuffer flagsBuffer;


    /***********************
     * UDPComm.designer.cs *
     ***********************/
private:
    QUdpSocket *udpSocket = NULL;
    // Simulation object when GPS hasn't be in QtAoG

    CSim *sim;

public:
    bool isUDPServerOn = false;

    void startUDPServer();
    void stopUDPServer();

   /**********************
     * OpenGL.Designer.cs *
     **********************/
    void drawLightBar(double Width, double Height, double offlineDistance, const QMatrix4x4 &modelview, const QMatrix4x4 &projection);
    //void calcFrustum(QOpenGLFunctions_2_1 *gl);
    //transitioning to OPenGL ES; use our own computed matrices
    void calcFrustum(const QMatrix4x4 &mvp);

    void setZoom();
    void loadGLTextures();

private:
    // GPS for 5Hz
    int timerFor200ms;

    void setupGui();


    /**************
     * FormGPS.cs *
     **************/
public:
    QString speedMPH();
    QString speedKPH();
    void sectionCalcWidths();
    void processSectionOnOffRequests();
    void scanForNMEA();

    /**************************
     * SerialComm.Designer.cs *
     **************************/
public:
    void autoSteerDataOutToPort();
    void autoSteerSettingsOutToPort();
    void sectionControlOutToPort();

    /***********************
     * GUI.Designer.cs *
     ***********************/
    void manualAllBtnsUpdate();
    void manualBtnUpdate(int sectNumber);
    void lineUpManualBtns();

    // formgps_ui
    bool closeAllMenus();

    // return data to QML

    Q_INVOKABLE QString Zone () const;
    Q_INVOKABLE QString FixNorthing () const;
    Q_INVOKABLE QString FixEasting () const;
    Q_INVOKABLE QString Latitude () const;
    Q_INVOKABLE QString Longitude () const;
    Q_INVOKABLE QString SatsTracked () const;
    Q_INVOKABLE QString HDOP () const;
    Q_INVOKABLE QString NMEAHz () const;
    Q_INVOKABLE QString PassNumber () const;
    Q_INVOKABLE QString CurveNumber () const;
    Q_INVOKABLE QString Heading () const;
    Q_INVOKABLE QString GPSHeading () const;
    Q_INVOKABLE QString Status () const;
    Q_INVOKABLE QString FixQuality () const;
    Q_INVOKABLE QString GyroInDegrees () const;
    Q_INVOKABLE QString RollInDegrees () const;
    Q_INVOKABLE QString SetSteerAngle() const;
    Q_INVOKABLE QString ActualSteerAngle() const;
    Q_INVOKABLE QString FixHeading () const;
    Q_INVOKABLE QString LookAhead () const;
    Q_INVOKABLE QString StepFixNum () const;
    Q_INVOKABLE QString CurrentStepDistance () const;
    Q_INVOKABLE QString TotalStepDistance () const;
    Q_INVOKABLE QString WorkSwitchValue () const;
    Q_INVOKABLE QString AgeDiff () const;
    Q_INVOKABLE QString SpeedMPH () const;
    Q_INVOKABLE QString SpeedKPH () const;
    Q_INVOKABLE QString XTE () const;
    Q_INVOKABLE QString inchXTE () const;
    Q_INVOKABLE QString FixOffset () const;
    Q_INVOKABLE QString FixOffSetInch() const;
    Q_INVOKABLE QString Altitude () const;
    Q_INVOKABLE QString AltitudeFeet () const;
    Q_INVOKABLE QString DistPivotM() const;
    Q_INVOKABLE QString DistPivotFt() const;

    /**************************
     * UI/Qt object callbacks *
     **************************/
public slots:
    /*******************
     * from FormGPS.cs *
     *******************/
    void onGLControl_clicked(const QVariant &event);

    void onBtnDisplayDrag_clicked();
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

    void onBtnTwoD_clicked();
    void onBtnThreeD_clicked();

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
     * Slot for receive nmea data, which is generated from CSim class
     */
    void on_csim_new_position(QByteArray new_nmea_data);

};

#endif // FORMGPS_H
