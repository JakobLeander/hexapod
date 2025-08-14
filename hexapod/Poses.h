#ifndef POSES_H
#define POSES_H
#include <Arduino.h>
#include "KeyFrame.h"

const int16_t MIN_HOME_HEIGHT = -60;  // Foot z vs body center in milimeters
const int16_t MAX_HOME_HEIGHT = -140; // Foot z vs body center in milimeters
const int16_t FOOT_HOME_HEIGHT = -60; // Foot z vs body center in milimeters
const int16_t FOOT_UP_HEIGHT = -30;   // Foot z vs body center in milimeters
const int16_t FOOT_SLEEP_HEIGHT = 0;  // Foot z vs body center in milimeters
const int16_t FOOT_FB_DISTANCE = 150; // Foot X vs body center in milimeters for front and back feet
const int16_t FOOT_WIDTH_FB = 123;    // Foot distance from body center to side feet in milimeters for front and back feet
const int16_t FOOT_WIDTH_M = 177;     // Foot distance from body center to side feet in milimeters for middle feet
const int16_t FOOT_WALK_X = 40;       // How much to move each foot back and forth in milimeters when walking
class Poses
{
public:
    KeyFrame Home(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame Sleep{
        .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .LMiddle = {0, FOOT_WIDTH_M, FOOT_SLEEP_HEIGHT},
        .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT},
        .RMiddle = {0, -FOOT_WIDTH_M, FOOT_SLEEP_HEIGHT},
        .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, FOOT_SLEEP_HEIGHT}};
    KeyFrame W0(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_up_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_up_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_up_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame W1(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;

        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0 - FOOT_WALK_X, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0 + FOOT_WALK_X, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame W2(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;

        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_up_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_up_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_up_height}};
        return frame;
    }
    KeyFrame W3(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0 + FOOT_WALK_X, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE - FOOT_WALK_X, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0 - FOOT_WALK_X, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE + FOOT_WALK_X, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame R0(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_up_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_up_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_up_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }

    KeyFrame R1(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {99, 167, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-183, 64, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {61, -166, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }

    KeyFrame R2(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M, foot_up_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_up_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_up_height}};
        return frame;
    }

    KeyFrame R3(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        int16_t foot_up_height = foot_home_height + 30;
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {-61, 166, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {183, -64, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-99, -167, foot_home_height}};
        return frame;
    }

    KeyFrame A1_1(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {60, FOOT_WIDTH_M, foot_home_height + 40},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {60, -FOOT_WIDTH_M, foot_home_height + 40},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame A1_2(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {60, FOOT_WIDTH_M, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {60, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame A1_3(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE + 40, FOOT_WIDTH_FB - 70, foot_home_height + 50},
            .LMiddle = {60, FOOT_WIDTH_M, foot_home_height - 30},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE + 40, -FOOT_WIDTH_FB + 70, foot_home_height + 50},
            .RMiddle = {60, -FOOT_WIDTH_M, foot_home_height - 30},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
    KeyFrame A1_4(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE + 40, FOOT_WIDTH_FB - 70, foot_home_height - 40},
            .LMiddle = {60, FOOT_WIDTH_M, foot_home_height - 30},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE + 40, -FOOT_WIDTH_FB + 70, foot_home_height - 40},
            .RMiddle = {60, -FOOT_WIDTH_M, foot_home_height - 30},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }

    KeyFrame A2_1(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M+50, foot_home_height+50},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }

    KeyFrame A2_2(uint8_t height)
    {
        int16_t foot_home_height = map(height, 0, 9, MIN_HOME_HEIGHT, MAX_HOME_HEIGHT);
        KeyFrame frame{
            .LFront = {FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .LMiddle = {0, FOOT_WIDTH_M+50, foot_home_height},
            .LBack = {-FOOT_FB_DISTANCE, FOOT_WIDTH_FB, foot_home_height},
            .RFront = {FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height},
            .RMiddle = {0, -FOOT_WIDTH_M, foot_home_height},
            .RBack = {-FOOT_FB_DISTANCE, -FOOT_WIDTH_FB, foot_home_height}};
        return frame;
    }
};
#endif