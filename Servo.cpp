#include "Servo.h"

void Servo::setTargetAngle(float angle)
{
    angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
    if (m_isReverse)
    {
        angle = -angle;
    }
    m_target = map(angle, MIN_ANGLE, MAX_ANGLE, TARGET_MIN, TARGET_MAX);
}