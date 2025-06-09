#ifndef SERVO_H
#define SERVO_H
#include <Arduino.h>

class Servo
{
private:
    const uint16_t PULSE_WIDTH_MIN = 576;  // Also set in pololu control center
    const uint16_t PULSE_WIDTH_MAX = 2464; // Also set in pololu control center
    const uint16_t PULSE_WIDTH_CENTER = PULSE_WIDTH_MIN + (PULSE_WIDTH_MAX - PULSE_WIDTH_MIN) / 2;
    const float MIN_ANGLE = -90.0;
    const float MAX_ANGLE = +90.0;
    const uint8_t MIN_SPEED = 0;
    const uint8_t MAX_SPEED = 100;

    String m_id = "";
    uint8_t m_pololuChannel = 0;
    bool m_isReverse = false;
    uint8_t m_speed = MIN_SPEED;
    int16_t m_pulseWidth = PULSE_WIDTH_CENTER;
    float m_angle = 0.0; // Current angle in degrees

public:
    void initialize(String id, uint8_t pololuChannel, bool isReverse = false);

    /// @brief Target angle
    /// @param angle Angle in degrees between -90 and +90
    /// @return
    void setAngle(float angle);

    /// @brief Set speed
    /// @param speed Speed from 0 to 100, where 0 is stop and 100 is maximum speed
    /// @return
    void setSpeed(uint8_t speed);

    /// @brief Get id name of servo to make it easier to identify
    /// @return Servo id as a string
    String getId() const { return m_id; }

    /// @brief Get pololu channel number servo is connected to
    /// @return Pololu channel 0-17
    uint8_t getPololuChannel() const { return m_pololuChannel; }

    /// @brief Desired Servo angle in degree
    /// @return Target servo angle in degree
    float getTargetAngle() const { return m_angle; }

    /// @brief Desired Servo position as pulsewidth
    /// @return Target servo pulsewidth
    uint16_t getTargetPosition() const { return m_pulseWidth; }

    /// @brief Desired Servo Speed
    /// @return Target servo speed (0-100)
    uint8_t getTargetSpeed() const { return m_speed; }

    /// @brief Is reversed
    /// @return Is reverse
    bool isReverse() const { return m_isReverse; }
};

#endif