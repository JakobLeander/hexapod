#ifndef POSES_H
#define POSES_H
#include <Arduino.h>
#include "KeyFrame.h"

const int16_t FOOT_HOME_HEIGHT = -60; // Foot z vs body center in milimeters
const int16_t FOOT_UP_HEIGHT = -30;   // Foot z vs body center in milimeters
const int16_t FOOT_SLEEP_HEIGHT = 0;  // Foot z vs body center in milimeters
const int16_t FOOT_FB_DISTANCE = 150; // Foot X vs body center in milimeters for front and back feet
const int16_t FOOT_WIDTH_FB = 123;    // Foot distance from body center to side feet in milimeters for front and back feet
const int16_t FOOT_WIDTH_M = 177;     // Foot distance from body center to side feet in milimeters for middle feet
const int16_t FOOT_WALK_X = 40;       // How much to move each foot back and forth in milimeters when walking
struct Poses
{
    KeyFrame Home{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame Sleep{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_SLEEP_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_SLEEP_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT}};
    KeyFrame W0{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_UP_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame W1{
        .LFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0 - FOOT_WALK_X, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0 + FOOT_WALK_X, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame W2{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_UP_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_UP_HEIGHT}};
    KeyFrame W3{
        .LFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0 + FOOT_WALK_X, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0 - FOOT_WALK_X, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame R0{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_UP_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame R1{
        .LFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0 + FOOT_WALK_X, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0 + FOOT_WALK_X, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
    KeyFrame R2{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_UP_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_UP_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_UP_HEIGHT}};
    KeyFrame R3{
        .LFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .LMiddle = {0 - FOOT_WALK_X, FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT},
        .RMiddle = {0 - FOOT_WALK_X, -FOOT_WIDTH_M, FOOT_HOME_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, FOOT_HOME_HEIGHT}};
};
#endif