#ifndef KEYFRAME_H
#define KEYFRAME_H
#include <Arduino.h>
#include "Position.h"

struct KeyFrame
{
    Position LFront;
    Position LMiddle;
    Position LBack;
    Position RFront;
    Position RMiddle;
    Position RBack;
};
#endif