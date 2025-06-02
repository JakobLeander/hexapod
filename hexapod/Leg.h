#ifndef LEG_H
#define LEG_H
#include <Arduino.h>
#include "Servo.h"
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
    int16_t m_anchorX = 0;     // X coordinate of leg anchor point to body
    int16_t m_anchorY = 0;     // Y coordinate of leg anchor point to body
    int16_t m_anchorZ = 0;     // Z coordinate of leg anchor point to body
    int16_t m_anchorAngle = 0; // Angle to bodys y axis, 0 is middle leg

    Servo m_footServo; // Foot servo
    Servo m_kneeServo; // Knee servo
    Servo m_hipServo;  // Hip servo

    const int16_t THIGH_LENGTH = 84; // J2L: Length of leg segment from knee servo horn to foot servo horn
    const int16_t FOOT_LENGTH = 127; // J3L Length of leg segment from foot servo horn to toe

public:
    /// @brief Initialize leg with id and anchor position
    /// @param id id of the leg
    /// @param anchorX shoulder joint position in milimeters from body center
    /// @param anchorY shoulder joint position in milimeters from body center
    /// @param anchorAngle angle to bodys x axis, x is forward
    // z position is assumed in height with knee joint and is static vs body
    Leg(LegId id, int16_t anchorX, int16_t anchorY, float anchorAngle);

    /// @brief Set foot position from body center
    /// @param x vs body center in milimeters
    /// @param y vs body center in milimeters
    /// @param z vs body center in milimeters
    /// @return
    void setFootPosition(int16_t footPosX, int16_t footPosY, int16_t footPosZ);
};

#endif