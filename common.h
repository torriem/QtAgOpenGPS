#ifndef COMMON_H
#define COMMON_H

// we'll put common constants etc in here.
const int MAXSECTIONS = 9;

#pragma pack(push,1)
struct LookAheadPixels {
    uchar red;
    uchar green;
    uchar blue;
    uchar alpha;
};

enum hydStates {
    down=0, up=1
};
#pragma pack(pop)

#endif // COMMON_H
