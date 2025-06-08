#include "Leg.h"

Leg::Leg(LegId id, int16_t anchorX, int16_t anchorY)
    : m_legId(id), m_anchorX(anchorX), m_anchorY(anchorY)
{
    // calculate anchor angle in degree
    m_anchorAngleDeg = atan2(m_anchorY, m_anchorX) * 180.0 / M_PI;

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

bool Leg::setFootPosition(int16_t footPosX, int16_t footPosY, int16_t footPosZ, uint16_t speed)
{
    // TODO handle legs initial angle vs body
    Serial.println("Anchor Position: x=" + String(m_anchorX) + ", y=" + String(m_anchorY) + ", z=" + String(m_anchorZ));

    Serial.println("Foot position vs body: x=" + String(footPosX) + ", y=" + String(footPosY) + ", z=" + String(footPosZ));

    // Calculate foot position relative to anchor
    float x = footPosX - m_anchorX; // X coordinate relative to anchor
    float y = footPosY - m_anchorY; // Y coordinate relative to anchor
    float z = footPosZ - m_anchorZ; // Z coordinate relative to anchor
    Serial.println("Foot position vs anchor: x=" + String(x) + ", y=" + String(y) + ", z=" + String(z));

    // Angle to turn hip vs x axis
    float hipAngleDeg = atan2(y, x) * 180.0 / M_PI;

    // Calculate servo angle
    hipAngleDeg -= m_anchorAngleDeg;

    // Since knee joint is origo for rest of calculation calcuate its position
    // it is closer to foot than hip joint
    float len = sqrt(sq(x) + sq(y));
    float offsetX = x * HIP_TO_KNEE_LENGTH / len;
    float offsetY = y * HIP_TO_KNEE_LENGTH / len;
    Serial.println("Offset: x=" + String(offsetX) + ", y=" + String(offsetY));
    x = x - offsetX;
    y = y - offsetY;
    Serial.println("Foot position vs knee joint: x=" + String(x) + ", y=" + String(y) + ", z=" + String(z));

    // Calculate angles for knee and foot servos
    float h = sqrt(sq(x) + sq(y));
    Serial.println("h: " + String(h));

    float l = sqrt(sq(h) + sq(z));
    Serial.println("l: " + String(l));

    float footAngleDeg = acos((sq(THIGH_LENGTH) + sq(FOOT_LENGTH) - sq(l)) / (2 * THIGH_LENGTH * FOOT_LENGTH)) * RAD2DEG;

    // Since servo is calibrated to 0 when angle is 90 degree subtract  90 degree
    footAngleDeg = footAngleDeg - 90;
    Serial.println("footAngleDeg: " + String(footAngleDeg));

    float vB = acos((sq(l) + sq(THIGH_LENGTH) - sq(FOOT_LENGTH)) / (2 * l * THIGH_LENGTH));
    Serial.println("vB: " + String(vB * RAD2DEG));

    float vA = atan2(z, h);
    Serial.println(" vA: " + String(vA * RAD2DEG));

    float kneeAngleDeg = (vB + vA) * RAD2DEG;
    Serial.println("kneeAngleDeg: " + String(kneeAngleDeg));

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

    // Calculate speeds based on current and target angles
    float hipAngleDiff = abs(hipAngleDeg - hipCurrentAngle);
    float kneeAngleDiff = abs(kneeAngleDeg - kneeCurrentAngle);
    float footAngleDiff = abs(footAngleDeg - footCurrentAngle);

    // find largest diff
    float largestDiff = hipAngleDiff;
    if (kneeAngleDiff > largestDiff)
    {
        largestDiff = kneeAngleDiff;
    }
    if (footAngleDiff > largestDiff)
    {
        largestDiff = footAngleDiff;
    }

    // Calculate servo speeds
    // If servos need to move differently adjust speed so all servos reach their target at the same time
    float hipSpeed = speed;
    float kneeSpeed = speed;
    float footSpeed = speed;

    if (largestDiff > 0)
    {
        hipSpeed = (hipAngleDiff / largestDiff) * speed;
        kneeSpeed = (kneeAngleDiff / largestDiff) * speed;
        footSpeed = (footAngleDiff / largestDiff) * speed;
    }

    // Set speeds to servos
    m_hipServo.setSpeed(hipSpeed);
    m_kneeServo.setSpeed(kneeSpeed);
    m_footServo.setSpeed(footSpeed);

    return true;
}