#include "Servo.h"
#include "Leg.h"
#include "Poses.h"
#include "KeyFrame.h"
#include "Position.h"

#include <PololuMaestro.h> // https://www.pololu.com/docs/0J40/5.e
#include <SoftwareSerial.h>
#define RX_PIN 10                      // Pin that is connected to Maestro TX pin
#define TX_PIN 11                      // Pin that is connected to Maestro RX pin
const uint16_t SERVO_ACCELERATION = 0; // Unit is 1/4 microsecond
const uint8_t ROBOT_SPEED = 15;        // TODO: Make dynamic

SoftwareSerial m_MaestroSerial(RX_PIN, TX_PIN);
MiniMaestro m_maestro(m_MaestroSerial);
Poses m_poses;

enum HexapodState
{
    SLEEP,
    HOME,
    WALKFORWARD,
    WALKBACKWARD,
    ROTATELEFT,
    ROTATERIGHT,
};

// Initialize legs with their respective anchor positions
Leg m_LFrontLeg(LegId::LFRONT, 74, 39);
Leg m_LMiddleLeg(LegId::LMIDDLE, 0, 64);
Leg m_LBackLeg(LegId::LBACK, -74, 39);
Leg m_RFrontLeg(LegId::RFRONT, 74, -39);
Leg m_RMiddleLeg(LegId::RMIDDLE, 0, -64);
Leg m_RBackLeg(LegId::RBACK, -74, -39);

KeyFrame m_currentKeyFrame = {m_poses.Home};
KeyFrame m_targetKeyFrame = {m_poses.Home};

// TODO: Make dynamic
// Since our body center is higher than joint it cannot be zero
int16_t ROBOT_HEIGHT = 127; // Desired height of the robot body above floor in millimeters
const int8_t INTERPOLATIONS = 5;
int8_t m_interpolationCount = -1;
uint8_t m_walkCycleCount = 0;

char btCommand = '\0';

HexapodState m_hexapodState = HexapodState::SLEEP;
HexapodState m_hexapodDesiredState = HexapodState::SLEEP;

void setup()
{
    Serial.begin(115200);
    m_MaestroSerial.begin(115200);
    delay(1000);

    Serial.println("Ready!");
}

void loop()
{
    if (Serial.available() > 0)
    {
        btCommand = Serial.read();
        Serial.println("Received command: " + String(btCommand));
    }

    if (btCommand == 'H')
    {
        m_hexapodDesiredState = HexapodState::HOME;
    }

    if (btCommand == 'S')
    {
        m_hexapodDesiredState = HexapodState::SLEEP;
    }

    if (btCommand == 'F')
    {
        m_hexapodDesiredState = HexapodState::WALKFORWARD;
    }
    if (btCommand == 'B')
    {
        m_hexapodDesiredState = HexapodState::WALKBACKWARD;
    }
    if (btCommand == 'L')
    {
        m_hexapodDesiredState = HexapodState::ROTATELEFT;
    }

    if (btCommand == 'R')
    {
        m_hexapodDesiredState = HexapodState::ROTATERIGHT;
    }

    // Do interpolation if we are in middle of a move
    if (m_interpolationCount >= 0 && not IsServosMoving())
    {
        Interpolate();

        // Commit all legs to write values to servos
        CommitAllLegs();

        if (m_interpolationCount == 0)
        {
            // Update current key frame to target key frame
            m_currentKeyFrame = m_targetKeyFrame;
        }
        m_interpolationCount--;
    }

    if (m_interpolationCount < 0 && not IsServosMoving())
    {
        DetermineNextMove();
    }
}

void DetermineNextMove()
{
    // Trigger a state change if requested
    if (m_hexapodDesiredState != m_hexapodState)
    {
        // for some states just do the change no danger
        if (m_hexapodState == HexapodState::HOME || m_hexapodState == HexapodState::SLEEP)
        {
            m_hexapodState = m_hexapodDesiredState;
            m_walkCycleCount = 0;
        }

        // For gait state only allow changes when legs are in middle position
        // To check for completion of cycle 0 or 2 we look for cycle 1 or 3
        if (m_hexapodState == HexapodState::WALKFORWARD || m_hexapodState == HexapodState::WALKBACKWARD || m_hexapodState == HexapodState::ROTATELEFT || m_hexapodState == HexapodState::ROTATERIGHT)
        {
            if (1 == m_walkCycleCount || 3 == m_walkCycleCount)
            {
                m_hexapodState = m_hexapodDesiredState;

                // if new state is another cycle keep cyclecount so we can fluently shift, otherwise zero cycle count
                if (m_hexapodState == HexapodState::HOME || m_hexapodState == HexapodState::SLEEP)
                {
                    m_walkCycleCount = 0;
                }
            }
        }
    }

    switch (m_hexapodState)
    {
    case HexapodState::SLEEP:
        MoveKeyFrame(m_poses.Sleep);
        break;

    case HexapodState::HOME:
        MoveKeyFrame(m_poses.Home);
        break;

    case HexapodState::WALKFORWARD:
        switch (m_walkCycleCount)
        {
        case 0:
            MoveKeyFrame(m_poses.W0);
            break;
        case 1:
            MoveKeyFrame(m_poses.W1);
            break;
        case 2:
            MoveKeyFrame(m_poses.W2);
            break;
        case 3:
            MoveKeyFrame(m_poses.W3);
            break;
        }

        m_walkCycleCount++;
        if (m_walkCycleCount > 3)
        {
            m_walkCycleCount = 0;
        }
        break;
    case HexapodState::WALKBACKWARD:
        switch (m_walkCycleCount)
        {
        case 0:
            MoveKeyFrame(m_poses.W0);
            break;
        case 1:
            MoveKeyFrame(m_poses.W3);
            break;
        case 2:
            MoveKeyFrame(m_poses.W2);
            break;
        case 3:
            MoveKeyFrame(m_poses.W1);
            break;
        }

        m_walkCycleCount++;
        if (m_walkCycleCount > 3)
        {
            m_walkCycleCount = 0;
        }
        break;
    case HexapodState::ROTATELEFT:
        switch (m_walkCycleCount)
        {
        case 0:
            MoveKeyFrame(m_poses.R0);
            break;
        case 1:
            MoveKeyFrame(m_poses.R1);
            break;
        case 2:
            MoveKeyFrame(m_poses.R2);
            break;
        case 3:
            MoveKeyFrame(m_poses.R3);
            break;
        }

        m_walkCycleCount++;
        if (m_walkCycleCount > 3)
        {
            m_walkCycleCount = 0;
        }
        break;

    case HexapodState::ROTATERIGHT:
        switch (m_walkCycleCount)
        {
        case 0:
            MoveKeyFrame(m_poses.R0);
            break;
        case 1:
            MoveKeyFrame(m_poses.R3);
            break;
        case 2:
            MoveKeyFrame(m_poses.R2);
            break;
        case 3:
            MoveKeyFrame(m_poses.R1);
            break;
        }

        m_walkCycleCount++;
        if (m_walkCycleCount > 3)
        {
            m_walkCycleCount = 0;
        }
        break;
    }
}

// Interpolate to calculate a sequence of movements that performs desired move
void Interpolate()
{
    // Serial.println("InterpolationCount: " + String(m_interpolationCount));
    if (m_interpolationCount < 0)
    {
        return;
    }

    float stepFactor = ((float)INTERPOLATIONS - (float)m_interpolationCount) / (float)INTERPOLATIONS; // Calculate step factor from 0 to 1

    Position lFrontPosition = InterpolatePosition(m_currentKeyFrame.LFront, m_targetKeyFrame.LFront, stepFactor);
    m_LFrontLeg.setFootPosition(lFrontPosition, ROBOT_SPEED);

    Position lMiddlePosition = InterpolatePosition(m_currentKeyFrame.LMiddle, m_targetKeyFrame.LMiddle, stepFactor);
    m_LMiddleLeg.setFootPosition(lMiddlePosition, ROBOT_SPEED);

    Position lBackPosition = InterpolatePosition(m_currentKeyFrame.LBack, m_targetKeyFrame.LBack, stepFactor);
    m_LBackLeg.setFootPosition(lBackPosition, ROBOT_SPEED);

    Position rFrontPosition = InterpolatePosition(m_currentKeyFrame.RFront, m_targetKeyFrame.RFront, stepFactor);
    m_RFrontLeg.setFootPosition(rFrontPosition, ROBOT_SPEED);

    Position rMiddlePosition = InterpolatePosition(m_currentKeyFrame.RMiddle, m_targetKeyFrame.RMiddle, stepFactor);
    m_RMiddleLeg.setFootPosition(rMiddlePosition, ROBOT_SPEED);

    Position rBackPosition = InterpolatePosition(m_currentKeyFrame.RBack, m_targetKeyFrame.RBack, stepFactor);
    m_RBackLeg.setFootPosition(rBackPosition, ROBOT_SPEED);
}

Position InterpolatePosition(Position originalPosition, Position targetPosition, float stepFactor)
{
    Position interpolatePosition = {originalPosition.x + (targetPosition.x - originalPosition.x) * stepFactor,
                                    originalPosition.y + (targetPosition.y - originalPosition.y) * stepFactor,
                                    originalPosition.z + (targetPosition.z - originalPosition.z) * stepFactor};
    return interpolatePosition;
}

void ZeroAllLegs()
{
    // Set all servos to zero angle
    m_LFrontLeg.zeroAngles(ROBOT_SPEED);
    m_LMiddleLeg.zeroAngles(ROBOT_SPEED);
    m_LBackLeg.zeroAngles(ROBOT_SPEED);
    m_RFrontLeg.zeroAngles(ROBOT_SPEED);
    m_RMiddleLeg.zeroAngles(ROBOT_SPEED);
    m_RBackLeg.zeroAngles(ROBOT_SPEED);

    // Commit all legs to write values to servos
    CommitAllLegs();
}

void MoveKeyFrame(KeyFrame keyFrame)
{
    m_targetKeyFrame = keyFrame;
    m_interpolationCount = INTERPOLATIONS;
}

bool IsServosMoving()
{
    bool result = false;
    uint8_t movingState = m_maestro.getMovingState();
    if (movingState > 0)
    {
        result = true;
    }
    return result;
}

void CommitAllLegs()
{
    // Write values to all servos of all legs
    CommitLeg(m_LFrontLeg);
    CommitLeg(m_LMiddleLeg);
    CommitLeg(m_LBackLeg);
    CommitLeg(m_RFrontLeg);
    CommitLeg(m_RMiddleLeg);
    CommitLeg(m_RBackLeg);
}

void CommitLeg(Leg &leg)
{
    // Write values to all servos of the leg
    CommitServo(leg.getHipServo());
    CommitServo(leg.getKneeServo());
    CommitServo(leg.getFootServo());
}

void SetSpeed()
{
}

/// <summary>
/// Write values to a single servo
/// </summary>
/// <param name="srv">Servo to write</param>
void CommitServo(Servo servo)
{
    /*
    // during testing print values
    Serial.print(servo.getId() + ": ");

    Serial.print(": CH=" + (String)servo.getPololuChannel());

    if (servo.isReverse())
    {
        Serial.print(" REV=Y");
    }
    else
    {
        Serial.print(" REV=N");
    }

    Serial.print(" SPD=" + (String)servo.getTargetSpeed());
    Serial.print(" ANG=");
    Serial.print((String)servo.getTargetAngle());
    Serial.println(" PW=" + (String)servo.getTargetPosition());
    */
    m_maestro.setSpeed(servo.getPololuChannel(), servo.getTargetSpeed());
    m_maestro.setAcceleration(servo.getPololuChannel(), SERVO_ACCELERATION);
    uint16_t pololuPulseWidthTarget = servo.getTargetPosition() * 4;
    m_maestro.setTarget(servo.getPololuChannel(), pololuPulseWidthTarget);
}