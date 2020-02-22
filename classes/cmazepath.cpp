#include "cmazepath.h"

CMazePath::CMazePath(int iRows, int iCols, int *_iMaze) :
    mazeGrid(_iMaze), numCols(iCols), numMax(iRows*iCols)
{

}

QVector<Vec3> CMazePath::search(int iFromY, int iFromX, int iToY, int iToX)
{
    QVector<Vec3> mazeList;

    int iStart = (int)((iFromY * numCols) + iFromX);
    int iStop = (int)((iToY * numCols) + iToX);

    const int empty = 0;

    int Queue[numMax];
    int Origin[numMax];
    int iFront = 0, iRear = 0;

    //check if starting and ending points are valid (open)
    if (mazeGrid[iStart] != empty || mazeGrid[iStop] != empty)
    {
        mazeList.clear();
        return mazeList;
    }

    //create dummy array for storing status
    int iMazeStatus[numMax];
    //initially all nodes are ready
    for (int i = 0; i < numMax; i++)
        iMazeStatus[i] = (int)mazePathStatus::Ready;

    //add starting node to Q
    Queue[iRear] = iStart;
    Origin[iRear] = -1;
    iRear++;
    int iCurrent, iLeft, iRight, iUp, iDown, iRightUp, iRightDown, iLeftUp, iLeftDown;
    while (iFront != iRear) // while Q not empty
    {
        if (Queue[iFront] == iStop)     // maze is solved
            break;

        iCurrent = Queue[iFront];

        iLeft = iCurrent - 1;
        if (iLeft >= 0 && iLeft / numCols == iCurrent / numCols)    //if left node exists
        {
            if (mazeGrid[iLeft] == empty)   //if left node is open(a path exists)
            {
                if (iMazeStatus[iLeft] == (int)mazePathStatus::Ready)   //if left node is ready
                {
                    Queue[iRear] = iLeft; //add to Q
                    Origin[iRear] = iCurrent;
                    iMazeStatus[iLeft] = (int)mazePathStatus::Waiting; //change status to waiting
                    iRear++;
                }
            }
        }

        iRight = iCurrent + 1;
        if (iRight < numMax && iRight / numCols == iCurrent / numCols)    //if right node exists
        {
            if (mazeGrid[iRight] == empty)  //if right node is open(a path exists)
            {
                if (iMazeStatus[iRight] == (int)mazePathStatus::Ready)  //if right node is ready
                {
                    Queue[iRear] = iRight; //add to Q
                    Origin[iRear] = iCurrent;
                    iMazeStatus[iRight] = (int)mazePathStatus::Waiting; //change status to waiting
                    iRear++;
                }
            }
        }

        iUp = iCurrent + numCols;
        if (iUp < numMax)  //if top node exists
        {
            if (mazeGrid[iUp] == empty)    //if top node is open(a path exists)
            {
                if (iMazeStatus[iUp] == (int)mazePathStatus::Ready)    //if top node is ready
                {
                    Queue[iRear] = iUp; //add to Q
                    Origin[iRear] = iCurrent;
                    iMazeStatus[iUp] = (int)mazePathStatus::Waiting; //change status to waiting
                    iRear++;
                }
            }
        }

        iDown = iCurrent - numCols;
        if (iDown >= 0)   //if bottom node exists
        {
            if (mazeGrid[iDown] == empty)   //if bottom node is open(a path exists)
            {
                if (iMazeStatus[iDown] == (int)mazePathStatus::Ready)   //if bottom node is ready
                {
                    Queue[iRear] = iDown; //add to Q
                    Origin[iRear] = iCurrent;
                    iMazeStatus[iDown] = (int)mazePathStatus::Waiting; //change status to waiting
                    iRear++;
                }
            }
        }
        //if (diagonal == true)
        {
            iRightDown = iCurrent - numCols + 1;
            if (iRightDown < numMax && iRightDown >= 0 && iRightDown / numCols == (iCurrent / numCols) - 1)     //if bottom-right node exists
            {
                if (mazeGrid[iRightDown] == empty)  //if this node is open(a path exists)
                {
                    if (iMazeStatus[iRightDown] == (int)mazePathStatus::Ready)  //if this node is ready
                    {
                        Queue[iRear] = iRightDown; //add to Q
                        Origin[iRear] = iCurrent;
                        iMazeStatus[iRightDown] = (int)mazePathStatus::Waiting; //change status to waiting
                        iRear++;
                    }
                }
            }

            iRightUp = iCurrent + numCols + 1;
            if (iRightUp >= 0 && iRightUp < numMax && iRightUp / numCols == (iCurrent / numCols) + 1)   //if upper-right node exists
            {
                if (mazeGrid[iRightUp] == empty)    //if this node is open(a path exists)
                {
                    if (iMazeStatus[iRightUp] == (int)mazePathStatus::Ready)    //if this node is ready
                    {
                        Queue[iRear] = iRightUp; //add to Q
                        Origin[iRear] = iCurrent;
                        iMazeStatus[iRightUp] = (int)mazePathStatus::Waiting; //change status to waiting
                        iRear++;
                    }
                }
            }

            iLeftDown = iCurrent - numCols - 1;
            if (iLeftDown < numMax && iLeftDown >= 0 && iLeftDown / numCols == (iCurrent / numCols) - 1)    //if bottom-left node exists
            {
                if (mazeGrid[iLeftDown] == empty)   //if this node is open(a path exists)
                {
                    if (iMazeStatus[iLeftDown] == (int)mazePathStatus::Ready)   //if this node is ready
                    {
                        Queue[iRear] = iLeftDown; //add to Q
                        Origin[iRear] = iCurrent;
                        iMazeStatus[iLeftDown] = (int)mazePathStatus::Waiting; //change status to waiting
                        iRear++;
                    }
                }
            }

            iLeftUp = iCurrent + numCols - 1;
            if (iLeftUp >= 0 && iLeftUp < numMax && iLeftUp / numCols == (iCurrent / numCols) + 1)  //if upper-left node exists
            {
                if (mazeGrid[iLeftUp] == empty)     //if this node is open(a path exists)
                {
                    if (iMazeStatus[iLeftUp] == (int)mazePathStatus::Ready) //if this node is ready
                    {
                        Queue[iRear] = iLeftUp; //add to Q
                        Origin[iRear] = iCurrent;
                        iMazeStatus[iLeftUp] = (int)mazePathStatus::Waiting; //change status to waiting
                        iRear++;
                    }
                }
            }
        }

        //change status of current node to processed
        iMazeStatus[iCurrent] = (int)mazePathStatus::Processed;
        iFront++;
    }

    Vec3 ptt;

    iCurrent = iStop;
    //Y
    ptt.northing = iCurrent / numCols;
    //X
    ptt.easting = iCurrent - (iCurrent / numCols * numCols);

    mazeList.clear();
    mazeList.append(ptt);

    for (int i = iFront; i >= 0; i--)
    {
        if (Queue[i] == iCurrent)
        {
            iCurrent = Origin[i];
            if (iCurrent == -1)     // maze is solved
                return mazeList;
            //add point
            ptt.northing = iCurrent / numCols; //Y
            ptt.easting = iCurrent - (iCurrent / numCols * numCols); //X
            mazeList.append(ptt);
        }
    }

    //no path exists
    mazeList.clear();
    return mazeList; //empty list

}
