#ifndef LEG_H
#define LEG_H
#include <Arduino.h>
#include "Servo.h"
#include "Poses.h"
#include "Position.h"
#include <Math.h>

enum class LegId : byte
{
    LFRONT,
    LMIDDLE,
    LBACK,
    RFRONT,
    RMIDDLE,
    RBACK
};

class Leg
{
private:
    LegId m_legId;
    int16_t m_anchorX = 0;        // X coordinate of leg anchor point to body
    int16_t m_anchorY = 0;        // Y coordinate of leg anchor point to body
    int16_t m_anchorZ = 0;        // Z coordinate of leg anchor point to body
    int16_t m_anchorAngleDeg = 0; // Angle to bodys x axis in degrees

    Servo m_footServo; // Foot servo
    Servo m_kneeServo; // Knee servo
    Servo m_hipServo;  // Hip servo
    int16_t m_footPositionX;
    int16_t m_footPositionY;
    int16_t m_footPositionZ;

    const int16_t THIGH_LENGTH = 84;       // J2L: Length of leg segment from knee servo horn to foot servo horn
    const int16_t FOOT_LENGTH = 127;       // J3L Length of leg segment from foot servo horn to toe
    const int16_t HIP_TO_KNEE_LENGTH = 28; // distance from hip servo horn to knee servo horn
    const float RAD2DEG = 180.0 / M_PI;

public:
    /// @brief Initialize leg with id and anchor position
    /// @param id id of the leg
    /// @param anchorX hip joint position in milimeters from body center
    /// @param anchorY hip joint position in milimeters from body center
    // z position is assumed in height with knee joint = 0 and is static vs body
    Leg(LegId id, int16_t anchorX, int16_t anchorY);

    Servo getFootServo() { return m_footServo; }
    Servo getKneeServo() { return m_kneeServo; }
    Servo getHipServo() { return m_hipServo; }

    Position getFootPosition(){return Position{m_footPositionX, m_footPositionY, m_footPositionZ};}

    /// @brief Set foot position from body center
    /// @param footPosition vs body center in milimeters
    /// @param speed how fast the robot should move
    /// @return true if position is valid and set, false if position is illegal
    bool setFootPosition(Position footPosition, int8_t speed);

    void zeroAngles(uint8_t speed)
    {
        // Set all servos to zero angle
        m_footServo.setAngle(0);
        m_kneeServo.setAngle(0);
        m_hipServo.setAngle(0);

        // Set speed for all servos
        m_footServo.setSpeed(speed);
        m_kneeServo.setSpeed(speed);
        m_hipServo.setSpeed(speed);
    }

    void identifyLeg(uint8_t speed)
    {
        // Set all servos to zero angle
        m_footServo.setAngle(10);
        m_kneeServo.setAngle(10);
        m_hipServo.setAngle(10);

        // Set speed for all servos
        m_footServo.setSpeed(speed);
        m_kneeServo.setSpeed(speed);
        m_hipServo.setSpeed(speed);
    }
};

#endif