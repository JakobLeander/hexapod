#include "Servo.h"

void Servo::initialize(String id, uint8_t pololuChannel, bool isReverse)
{
    m_id = id;
    m_pololuChannel = pololuChannel;
    m_isReverse = isReverse;

    // Set default values
    m_speed = MIN_SPEED;
    m_pulseWidth = PULSE_WIDTH_CENTER;
}

void Servo::setAngle(float angle)
{
    angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
    if (m_isReverse)
    {
        angle = -angle;
    }
    m_angle = angle;
    m_pulseWidth = map(angle, MIN_ANGLE, MAX_ANGLE, PULSE_WIDTH_MIN, PULSE_WIDTH_MAX);
}

void Servo::setSpeed(uint8_t speed)
{
    m_speed = constrain(m_speed, MIN_SPEED, MAX_SPEED);
}