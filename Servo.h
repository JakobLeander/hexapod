#ifndef SERVO_H
#define SERVO_H
#include <Arduino.h>

class Servo
{
private:
    const uint16_t PULSE_WIDTH_MIN = 576;            // Also set in pololu control center
    const uint16_t PULSE_WIDTH_MAX = 2464;           // Also set in pololu control center
    const uint16_t TARGET_MIN = PULSE_WIDTH_MIN * 4; // pulse width to transmit in units of quarter-microseconds, 0 means stop
    const uint16_t TARGET_MAX = PULSE_WIDTH_MAX * 4; // pulse width to transmit in units of quarter-microseconds, 0 means stop
    const float MIN_ANGLE = -90.0;
    const float MAX_ANGLE = +90.0;

    String m_id = "";
    int16_t m_target = 1520;
    uint8_t m_pololuChannel = 0;
    bool m_isReverse = false;

public:
    Servo(String id, uint8_t pololuChannel, bool isReverse = false)
        : m_id(id), m_pololuChannel(pololuChannel), m_isReverse(isReverse) {};

    /// @brief Target angle
    /// @param angle Angle in degrees between -90 and +90
    /// @return
    void setTargetAngle(float angle);
    String getId() const { return m_id; }
    uint8_t getPololuChannel() const { return m_pololuChannel; }

    /// @brief Target position to send to maestro in units of quarter-microseconds
    /// @return
    uint16_t getTarget() const { return m_target; }
};

#endif