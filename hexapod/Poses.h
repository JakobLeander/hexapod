#ifndef POSES_H
#define POSES_H
#include <Arduino.h>
#include "KeyFrame.h"

const int16_t FOOT_HOME_HEIGHT = -60; // Foot z vs body center in milimeters
const int16_t FOOT_SLEEP_HEIGHT = 0;  // Foot z vs body center in milimeters
struct Poses
{
    KeyFrame Home{
        .LFront = {150, 123, FOOT_HOME_HEIGHT},
        .LMiddle = {-60, 177, FOOT_HOME_HEIGHT},
        .LBack = {-150, 123, FOOT_HOME_HEIGHT},
        .RFront = {150, -123, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -177, FOOT_HOME_HEIGHT},
        .RBack = {-150, -123, FOOT_HOME_HEIGHT}};
    KeyFrame HomeLLegForward{
        .LFront = {150, 123, FOOT_HOME_HEIGHT},
        .LMiddle = {60, 177, FOOT_HOME_HEIGHT},
        .LBack = {-150, 123, FOOT_HOME_HEIGHT},
        .RFront = {150, -123, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -177, FOOT_HOME_HEIGHT},
        .RBack = {-150, -123, FOOT_HOME_HEIGHT}};
    KeyFrame HomeLLegBack{
        .LFront = {150, 123, FOOT_HOME_HEIGHT},
        .LMiddle = {-60, 177, FOOT_HOME_HEIGHT},
        .LBack = {-150, 123, FOOT_HOME_HEIGHT},
        .RFront = {150, -123, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -177, FOOT_HOME_HEIGHT},
        .RBack = {-150, -123, FOOT_HOME_HEIGHT}};

    KeyFrame Sleep{
        .LFront = {150, 123, FOOT_SLEEP_HEIGHT},
        .LMiddle = {0, 177, FOOT_SLEEP_HEIGHT},
        .LBack = {-150, 123, FOOT_SLEEP_HEIGHT},
        .RFront = {150, -123, FOOT_SLEEP_HEIGHT},
        .RMiddle = {0, -177, FOOT_SLEEP_HEIGHT},
        .RBack = {-150, -123, FOOT_SLEEP_HEIGHT}};
};
#endif