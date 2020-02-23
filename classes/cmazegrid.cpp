#include "cmazegrid.h"
#include "cgeofence.h"
#include "cboundary.h"
#include "cmazepath.h"
#include "glm.h"
#include "glutils.h"

CMazeGrid::CMazeGrid()
{
    mazeArr = NULL;
}

CMazeGrid::~CMazeGrid()
{
    if(mazeArr) delete mazeArr;
}

void CMazeGrid::buildMazeGridArray(const CBoundary &bnd, CGeoFence &gf,
                                   double minFieldX, double maxFieldX,
                                   double minFieldY, double maxFieldY)
{
    double mazeY = (maxFieldY - minFieldY);
    double mazeX = (maxFieldX - minFieldX);

    if (mazeY > mazeX) mazeScale = (int)(mazeY / 150);
    else mazeScale = (int)(mazeX / 150);

    if (mazeScale < 4) mazeScale = 4;
    //mazeScale = 4;

    mazeRowYDim = (int)mazeY / mazeScale;
    mazeColXDim = (int)mazeX / mazeScale;

    if(mazeArr) delete mazeArr;
    mazeArr = new int[mazeRowYDim * mazeColXDim];
    memset(mazeArr,0,sizeof(int) * mazeRowYDim * mazeColXDim);

    //row is Y, col is X   int[Y,X] [i,j] [row,col]
    Vec3 pot;

    //mf.yt.triggerDistanceOffset += mazeScale;
    //mf.turn.BuildTurnLines();

    int arr[mazeRowYDim][mazeColXDim];
    memset(arr,0,sizeof(int) * mazeRowYDim * mazeColXDim);

    for (int i = 0; i < mazeRowYDim; i++)
    {
        for (int j = 0; j < mazeColXDim; j++)
        {
            pot.easting = (j * mazeScale) + (int)minFieldX;
            pot.northing = (i * mazeScale) + (int)minFieldY;
            if (!gf.isPointInsideGeoFences(bnd, pot))
            {
                mazeArr[(i * mazeColXDim) + j] = 1;
                arr[i][j] = 1;
            }
            else
            {
                mazeArr[(i * mazeColXDim) + j] = 0;
                arr[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < mazeRowYDim; i++)
    {
        for (int j = 0; j < mazeColXDim; j++)
        {
            if (i > 0 && i < mazeRowYDim - 1 && j > 0 && j < mazeColXDim - 1)
            {
                if (arr[i][j] == 1 && arr[i + 1][j] == 0) arr[i + 1][j] = 2;
                if (arr[i][j] == 1 && arr[i - 1][j] == 0) arr[i - 1][j] = 2;
                if (arr[i][j] == 1 && arr[i][j + 1] == 0) arr[i][j + 1] = 2;
                if (arr[i][j] == 1 && arr[i][j - 1] == 0) arr[i][j - 1] = 2;
            }
        }
    }

    for (int i = 0; i < mazeRowYDim; i++)
    {
        for (int j = 0; j < mazeColXDim; j++)
        {
            if (arr[i][j] == 2)
            {
                mazeArr[(i * mazeColXDim) + j] = 1;
                arr[i][j] = 1;
            }
        }
    }

}

QVector<Vec3> CMazeGrid::searchForPath (double minFieldX, double minFieldY,
                                        const Vec3 start, const Vec3 stop)
{
    CMazePath maze(mazeRowYDim, mazeColXDim, mazeArr);

    QVector<Vec3> mazeList = maze.search((int)((start.northing - minFieldY) / mazeScale),
                                              (int)((start.easting - minFieldX) / mazeScale),
                                              (int)((stop.northing - minFieldY) / mazeScale),
                                              (int)((stop.easting - minFieldX) / mazeScale));

    if (mazeList.size() == 0) return mazeList;

    //we find our way back, we want to go forward, so reverse the list
    std::reverse(mazeList.begin(), mazeList.end());

    int cnt = mazeList.size();

    if (cnt < 3)
    {
        mazeList.clear();
        return mazeList;
    }

    //the temp array
    QVector<Vec3> arr2 = mazeList;

    mazeList.clear();

    for (int h = 0; h < cnt; h++)
    {
        arr2[h].easting = (arr2[h].easting * mazeScale) + minFieldX;
        arr2[h].northing = (arr2[h].northing * mazeScale) + minFieldY;
        mazeList.append(arr2[h]);
    }

    //fill in the gaps
    for (int i = 0; i < cnt; i++)
    {
        int j = i + 1;
        if (j == cnt)
            j = i;
        double distance = glm::distance(mazeList[i], mazeList[j]);
        if (distance > 2)
        {
            Vec3 pointB((mazeList[i].easting + mazeList[j].easting) / 2.0,
                        (mazeList[i].northing + mazeList[j].northing) / 2.0, 0);

            mazeList.insert(j, pointB);
            cnt = mazeList.size();
            i--; //go back to original point again
        }
    }

    cnt = mazeList.size();

    //the temp array
    Vec3 arr[cnt];

    //how many samples
    int smPts = mazeScale;

    //read the points before and after the setpoint
    for (int s = 0; s < smPts; s++)
    {
        arr[s].easting = mazeList[s].easting;
        arr[s].northing = mazeList[s].northing;
        arr[s].heading = mazeList[s].heading;
    }

    for (int s = cnt - smPts; s < cnt; s++)
    {
        arr[s].easting = mazeList[s].easting;
        arr[s].northing = mazeList[s].northing;
        arr[s].heading = mazeList[s].heading;
    }

    //average them - center weighted average
    for (int i = smPts; i < cnt - smPts; i++)
    {
        for (int j = -smPts; j < smPts; j++)
        {
            arr[i].easting += mazeList[j + i].easting;
            arr[i].northing += mazeList[j + i].northing;
        }
        arr[i].easting /= (smPts * 2);
        arr[i].northing /= (smPts * 2);
        arr[i].heading = mazeList[i].heading;
    }

    //clear the list and reload with calc headings - first and last droppped
    mazeList.clear();

    for (int i = mazeScale; i < cnt - mazeScale; i++)
    {
        Vec3 pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i].easting,
                            arr[i + 1].northing - arr[i].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        mazeList.append(pt3);
    }

    return mazeList;
}

void CMazeGrid::drawArr(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                        double minFieldX, double minFieldY)
{
    GLHelperColors gldraw;
    ColorVertex cv;

    int ptCount = mazeRowYDim * mazeColXDim;
    for (int h = 0; h < ptCount; h++)
    {
        if (mazeArr[h] == 1)
        {
            cv.color = QVector4D(0.0095f, 0.007520f, 0.97530f,1.0f);
            int Y = h / mazeColXDim; //Y
            int X = h - (h / mazeColXDim * mazeColXDim); //X
            cv.vertex = QVector3D((X * mazeScale) + (int)minFieldX, (Y * mazeScale) + (int)minFieldY, 0);
            gldraw.append(cv);
        }
        else
        {
            cv.color = QVector4D(0.95f, 0.7520f, 0.07530f, 1.0f);
            int Y = h / mazeColXDim; //Y
            int X = h - (h / mazeColXDim * mazeColXDim); //X
            cv.vertex = QVector3D((X * mazeScale) + (int)minFieldX, (Y * mazeScale) + (int)minFieldY, 0);
            gldraw.append(cv);
        }
    }

    gldraw.draw(gl, mvp, GL_POINTS, 2.0f);
}
