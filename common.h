#ifndef COMMON_H
#define COMMON_H

#include <qglobal.h>

// we'll put common constants etc in here.
const int MAXSECTIONS = 64;
const int MAXBOUNDARIES = 6;
const int MAXHEADS = 6;

#pragma pack(push,1)
struct LookAheadPixels {
    uchar red;
    uchar green;
    uchar blue;
    uchar alpha;
};
#pragma pack(pop)

enum hydStates {
    down=0, up=1
};

enum messageBox {
    info=0,
    warning=1,
    error=2,
    ackerror=3
};

#endif // COMMON_H
