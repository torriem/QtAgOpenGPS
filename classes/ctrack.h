#ifndef CTRACK_H
#define CTRACK_H

#include <QVector>
#include <QAbstractListModel>
#include "vec3.h"
#include "vec2.h"
#include "setter.h"
#include "cabcurve.h"
#include "cabline.h"

class QOpenGLFunctions;
class CVehicle;
class CABLine;
class CABCurve;
class CBoundary;
class CYouTurn;
class CAHRS;
class CGuidance;
class CNMEA;
class CCamera;
class CTram;


enum TrackMode {
    None = 0,
    AB = 2,
    Curve = 4,
    bndTrackOuter = 8,
    bndTrackInner = 16,
    bndCurve = 32,
    waterPivot = 64
};//, Heading, Circle, Spiral

class CTrk
{
public:
    QVector<Vec3> curvePts;
    double heading;
    QString name;
    bool isVisible;
    Vec2 ptA;
    Vec2 ptB;
    Vec2 endPtA;
    Vec2 endPtB;
    int mode;
    double nudgeDistance;

    CTrk();
};

class CTrack : public QAbstractListModel
{
    Q_OBJECT

public:
    enum RoleNames {
        index = Qt::UserRole,
        NameRole,
        IsVisibleRole,
        ModeRole,
        ptA,
        ptB,
        endPtA,
        endPtB,
        nudgeDistance
    };

    QVector<CTrk> gArr;
    CABCurve curve;
    CABLine ABLine;

    Q_PROPERTY(int idx MEMBER idx NOTIFY idxChanged)
    //put a pointer to ourselves as a model.  This class is both
    //a list model, and also a bunch of properties
    Q_PROPERTY(QObject* model READ getModel CONSTANT)

    int idx, autoTrack3SecTimer;

    Q_PROPERTY(bool isAutoTrack MEMBER isAutoTrack NOTIFY isAutoTrackChanged)
    Q_PROPERTY(bool isAutoSnapToPivot MEMBER isAutoSnapToPivot NOTIFY isAutoSnapToPivotChanged)
    Q_PROPERTY(bool isAutoSnapped MEMBER isAutoSnapped NOTIFY isAutoSnappedChanged)
    Q_PROPERTY(int howManyPathsAway READ getHowManyPathsAway)

    bool isLine, isAutoTrack = false, isAutoSnapToPivot = false, isAutoSnapped;

    explicit CTrack(QObject* parent = nullptr);

    // CTrack interface
    int FindClosestRefTrack(Vec3 pivot, const CVehicle &vehicle);
    void SwitchToClosestRefTrack(Vec3 pivot, const CVehicle &vehicle);

    void NudgeTrack(double dist);
    void NudgeDistanceReset();
    void SnapToPivot();
    void NudgeRefTrack(double dist);
    void NudgeRefABLine(double dist);
    void NudgeRefCurve(double distAway);

    void DrawTrackNew(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, const CCamera &camera);
    void DrawTrack(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   bool isFontOn,
                   CYouTurn &yt, const CCamera &camera
                   , const CGuidance &gyd);

    void BuildCurrentLine(Vec3 pivot,
                          double secondsSinceStart, bool isAutoSteerbtnOn, int &makeUTurnCounter,
                          CYouTurn &yt,
                          CVehicle &vehicle,
                          const CBoundary &bnd,
                          const CAHRS &ahrs,
                          CGuidance &gyd,
                          CNMEA &pn);

    void ResetCurveLine();

    int getHowManyPathsAway();

    void AddPathPoint(Vec3 point);

    SETTER(int, idx, setIdx)
    SETTER(bool, isAutoTrack, setIsAutoTrack)
    SETTER(bool, isAutoSnapToPivot, setIsAutoSnapToPivot)
    SETTER(bool, isAutoSnapped, setIsAutoSnapped)


    QObject *getModel() { return this;}

    // QML model interface
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

protected:
    // QML model interface
    virtual QHash<int, QByteArray> roleNames() const override;
signals:
    void idxChanged();
    void isAutoSnapToPivotChanged();
    void isAutoSnappedChanged();
    void isAutoTrackChanged();

public slots:
    void reloadModel() {
        //force QML to reload the model to reflect changes
        //that may have been made in C++ code.
        beginResetModel();
        endResetModel();
    }

private:
    // Used by QML model interface
    QHash<int, QByteArray> m_roleNames;
};

#endif // CTRACK_H
