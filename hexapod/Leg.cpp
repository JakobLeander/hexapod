#include "Leg.h"

Leg::Leg(LegId id, int16_t anchorX, int16_t anchorY)
    : m_legId(id), m_anchorX(anchorX), m_anchorY(anchorY)
{
    // calculate anchor angle in degree
    m_anchorAngleDeg = atan2(m_anchorY, m_anchorX) * 180.0 / M_PI;

    // Initialize servos based on leg id
    switch (id)
    {
    case LegId::LFRONT: ///
        m_footServo.initialize("LFFoot", 0, true);
        m_kneeServo.initialize("LFKnee", 1, false);
        m_hipServo.initialize("LFHip", 2, true);
        break;
    case LegId::LMIDDLE: // Ok
        m_footServo.initialize("LMFoot", 6, true);
        m_kneeServo.initialize("LMKnee", 7, false);
        m_hipServo.initialize("LMHip", 8, true);
        break;
    case LegId::LBACK:
        m_footServo.initialize("LBFoot", 12, true);
        m_kneeServo.initialize("LBKnee", 13, false);
        m_hipServo.initialize("LBHip", 14, true);
        break;
    case LegId::RFRONT: // OK
        m_footServo.initialize("RFFoot", 3, false);
        m_kneeServo.initialize("RFKnee", 4, true);
        m_hipServo.initialize("RFHip", 5, true);
        break;
    case LegId::RMIDDLE: // OK
        m_footServo.initialize("RMFoot", 9, false);
        m_kneeServo.initialize("RMKnee", 10, true);
        m_hipServo.initialize("RMHip", 11, true);
        break;
    case LegId::RBACK:
        m_footServo.initialize("RBFoot", 15, false);
        m_kneeServo.initialize("RBKnee", 16, true);
        m_hipServo.initialize("RBHip", 17, true);
        break;
    };
}

bool Leg::setFootPosition(Position footPosition, uint8_t speed)
{
    // Calculate foot position relative to anchor
    float x = footPosition.x - m_anchorX; // X coordinate relative to anchor
    float y = footPosition.y - m_anchorY; // Y coordinate relative to anchor
    float z = footPosition.z - m_anchorZ; // Z coordinate relative to anchor

    // Angle to turn hip vs x axis
    float hipAngleDeg = atan2(y, x) * 180.0 / M_PI;

    // Calculate servo angle
    hipAngleDeg -= m_anchorAngleDeg;

    // Since knee joint is origo for rest of calculation calculate its position
    // it is closer to foot than hip joint
    float len = sqrt(sq(x) + sq(y));
    float kneeOffsetX = x * HIP_TO_KNEE_LENGTH / len;
    float kneeOffsetY = y * HIP_TO_KNEE_LENGTH / len;
    float kneePosX = m_anchorX + kneeOffsetX;
    float kneePosY = m_anchorY + kneeOffsetY;

    // Calculate foot pos vs knee joint
    x = footPosition.x - kneePosX;
    y = footPosition.y - kneePosY;

    // Calculate angles for knee and foot servos
    float h = sqrt(sq(x) + sq(y));
    float l = sqrt(sq(h) + sq(z));
    float footAngleDeg = acos((sq(THIGH_LENGTH) + sq(FOOT_LENGTH) - sq(l)) / (2 * THIGH_LENGTH * FOOT_LENGTH)) * RAD2DEG;

    // Since servo is calibrated to 0 when angle is 90 degree subtract  90 degree
    footAngleDeg = footAngleDeg - 90;

    // Foot position is not below servo when at sero so subtract some degrees to make it so
    footAngleDeg = footAngleDeg - 14.0; //

    float vB = acos((sq(l) + sq(THIGH_LENGTH) - sq(FOOT_LENGTH)) / (2 * l * THIGH_LENGTH));

    float vA = atan2(z, h);
    float kneeAngleDeg = (vB + vA) * RAD2DEG;

    // Save current angles
    float hipCurrentAngle = m_hipServo.getTargetAngle();
    float kneeCurrentAngle = m_kneeServo.getTargetAngle();
    float footCurrentAngle = m_footServo.getTargetAngle();

    if (isnan(hipAngleDeg) || isnan(kneeAngleDeg) || isnan(footAngleDeg))
    {
        Serial.println("Invalid angles calculated, cannot set foot position.");
        return false;
    }

    // Write new angles to servos
    m_hipServo.setAngle(hipAngleDeg);
    m_kneeServo.setAngle(kneeAngleDeg);
    m_footServo.setAngle(footAngleDeg);

    // Set speeds to servos
    m_hipServo.setSpeed(speed);
    m_kneeServo.setSpeed(speed);
    m_footServo.setSpeed(speed);

    m_footPositionX = footPosition.x;
    m_footPositionY = footPosition.y;
    m_footPositionZ = footPosition.z;

    return true;
}