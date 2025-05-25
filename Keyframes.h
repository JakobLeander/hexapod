#include "frame.h"

struct KeyFrames
{
    // Constants used from Simplification
    const float FHipForward = 0.0;
    const float FHipBack = 0.0;
    const float MHipForward = 0.0;
    const float MHipBack = 0.0;
    const float BHipForward = 0.0;
    const float BHipBack = 0.0;
    const float KneeUp = 0.0;
    const float KneeDown = 0.0;
    const float FootUp = 0.0;
    const float FootDown = 0.0;

    Frame HomePose{
        .LBHipAngle = 0.0,
        .LBKneeAngle = 0.0,
        .LBFootAngle = 0.0,
        .LMHipAngle = 0.0,
        .LMKneeAngle = 0.0,
        .LMFootAngle = 0.0,
        .LFHipAngle = 0.0,
        .LFKneeAngle = 0.0,
        .LFFootAngle = 0.0,
        .RBHipAngle = 0.0,
        .RBKneeAngle = 0.0,
        .RBFootAngle = 0.0,
        .RMHipAngle = 0.0,
        .RMKneeAngle = 0.0,
        .RMFootAngle = 0.0,
        .RFHipAngle = 0.0,
        .RFKneeAngle = 0.0,
        .RFFootAngle = 0.0};

    Frame SleepPose{
        .LBHipAngle = 0.0,
        .LBKneeAngle = 0.0,
        .LBFootAngle = 0.0,
        .LMHipAngle = 0.0,
        .LMKneeAngle = 0.0,
        .LMFootAngle = 0.0,
        .LFHipAngle = 0.0,
        .LFKneeAngle = 0.0,
        .LFFootAngle = 0.0,
        .RBHipAngle = 0.0,
        .RBKneeAngle = 0.0,
        .RBFootAngle = 0.0,
        .RMHipAngle = 0.0,
        .RMKneeAngle = 0.0,
        .RMFootAngle = 0.0,
        .RFHipAngle = 0.0,
        .RFKneeAngle = 0.0,
        .RFFootAngle = 0.0};
};