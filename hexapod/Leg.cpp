#include "Leg.h"

Leg::Leg(LegId id, int16_t anchorX, int16_t anchorY, float anchorAngle)
    : m_legId(id), m_anchorX(anchorX), m_anchorY(anchorY), m_anchorAngle(anchorAngle)
{
    // Initialize servos based on leg id
    switch (id)
    {
    case LegId::LFRONT:
        m_footServo.initialize("LFFoot", 6, true);
        m_kneeServo.initialize("LFKnee", 7, false);
        m_hipServo.initialize("LFHip", 8, false);
        break;
    case LegId::LMIDDLE:
        m_footServo.initialize("LMFoot", 3, true);
        m_kneeServo.initialize("LMKnee", 4, false);
        m_hipServo.initialize("LMHip", 5, false);
        break;
    case LegId::LBACK:
        m_footServo.initialize("LBFoot", 0, true);
        m_kneeServo.initialize("LBKnee", 1, false);
        m_hipServo.initialize("LBHip", 2, false);
        break;
    case LegId::RFRONT:
        m_footServo.initialize("RFFoot", 9, false);
        m_kneeServo.initialize("RFKnee", 10, true);
        m_hipServo.initialize("RFHip", 11, true);
        break;
    case LegId::RMIDDLE:
        m_footServo.initialize("RMFoot", 12, false);
        m_kneeServo.initialize("RMKnee", 13, true);
        m_hipServo.initialize("RMHip", 14, true);
        break;
    case LegId::RBACK:
        m_footServo.initialize("RBFoot", 15, false);
        m_kneeServo.initialize("RBKnee", 16, true);
        m_hipServo.initialize("RBHip", 17, true);
        break;
    };
}

void Leg::setFootPosition(int16_t footPosX, int16_t footPosY, int16_t footPosZ)
{

    // TODO handle legs initial angle vs body

    // Calculate foot position relative to anchor
    int16_t x = footPosX - m_anchorX; // X coordinate relative to anchor
    int16_t y = footPosY - m_anchorY; // Y coordinate relative to anchor
    int16_t z = footPosZ - m_anchorZ; // Z coordinate relative to anchor

    // Angle to turn hip
    float hipAngleDeg = atan2(x, y) * 180.0 / M_PI;
    int16_t h = sqrt(sq(x) + sq(y)); // Hip distance to foot
    int16_t l = sqrt(sq(h) + sq(z));

    float footAngleDeg = acos((sq(THIGH_LENGTH) + sq(FOOT_LENGTH) - sq(l)) / (2 * THIGH_LENGTH * FOOT_LENGTH)) * 180.0 / M_PI;
    float vB = acos((sq(l) + sq(THIGH_LENGTH) - sq(FOOT_LENGTH)) / (2 * l * THIGH_LENGTH));
    float vA = atan2(z, h);

    float kneeAngleDeg = (vB - vA) * 180.0 / M_PI;
}