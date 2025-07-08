#include "Servo.h"
#include "Leg.h"
#include "Poses.h"
#include "KeyFrame.h"
#include "Position.h"
#include <PololuMaestro.h> // https://www.pololu.com/docs/0J40/5.e

#define m_bluetoothSerial Serial1
#define m_MaestroSerial Serial2

const uint16_t SERVO_ACCELERATION = 0; // Unit is 1/4 microsecond
const uint8_t ROBOT_SPEED = 20;        // TODO: Make dynamic

MiniMaestro m_maestro(m_MaestroSerial);

unsigned long m_lastCommand;          // Last time a command was received
unsigned long m_lastPoll;             // Last time a command was received
const unsigned long BREAK_TIME = 500; // Number of miliseconds to allow no commands before we break
const unsigned long POLL_TIME = 0;    // Number of miliseconds between bluetooth reads
const char ACTION_NOTHING = 'N';      // Nothing
const char ACTION_FORWARD = 'F';      // Walk forward
const char ACTION_BACKWARD = 'B';     // Walk backward
const char ACTION_RIGHT = 'R';        // Rotate right
const char ACTION_LEFT = 'L';         // Rotate left

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

// TODO: Make dynamic
// Since our body center is higher than joint it cannot be zero
int16_t ROBOT_HEIGHT = 127; // Desired height of the robot body above floor in millimeters
const int8_t INTERPOLATIONS = 5;
int8_t m_interpolationCount = -1;
uint8_t m_walkCycleCount = 0;

HexapodState m_hexapodState = HexapodState::SLEEP;
HexapodState m_hexapodDesiredState = HexapodState::SLEEP;
uint8_t m_hexapod_height = 5;         // 0 to 9
uint8_t m_hexapod_desired_height = 5; // 0 to 9

KeyFrame m_currentKeyFrame = {m_poses.Home(m_hexapod_height)};
KeyFrame m_targetKeyFrame = {m_poses.Home(m_hexapod_height)};

void setup()
{
    Serial.begin(115200);
    delay(1000);
    m_bluetoothSerial.begin(9600);
    delay(1000);
    m_MaestroSerial.begin(115200);
    delay(1000);
    m_lastCommand = millis();
    m_lastPoll = millis();
    Serial.println("Ready!");
}

void loop()
{
    byte action = ACTION_NOTHING;
    byte actionValue = 0;

    // If no request to move for a while, stop moving unless we are in sleep or home state
    if ((millis() - m_lastCommand) > BREAK_TIME)
    {
        if (m_hexapodState != HexapodState::SLEEP)
        {
            m_hexapodDesiredState = HexapodState::HOME;
        }

        m_lastCommand = millis();
    }

    // See if there are any new bluetooth commands
    if ((millis() - m_lastPoll) > POLL_TIME)
    {
        ReadBluetoothCommand(action, actionValue);
        m_lastPoll = millis();
    }

    // If action
    if (action != ACTION_NOTHING)
    {
        Serial.print("BTCMD: ");
        Serial.print(char(action));
        Serial.print("[");
        Serial.print(actionValue);
        Serial.println("]");
        m_lastCommand = millis();
    }

    // TODO: Use the speed value (actionValue) to set the speed of the robot

    // Height change
    if (action == 'Z')
    {
        m_hexapod_desired_height = actionValue;
        Serial.print(m_hexapod_desired_height);
    }

    // Home
    if (action == 'H')
    {
        m_hexapodDesiredState = HexapodState::HOME;
    }

    // Sleep
    if (action == 'S')
    {
        m_hexapodDesiredState = HexapodState::SLEEP;
    }

    // Forward
    if (action == 'F')
    {
        m_hexapodDesiredState = HexapodState::WALKFORWARD;
    }

    // Backward
    if (action == 'B')
    {
        m_hexapodDesiredState = HexapodState::WALKBACKWARD;
    }

    // Rotate left
    if (action == 'L')
    {
        m_hexapodDesiredState = HexapodState::ROTATELEFT;
    }

    // Rotate right
    if (action == 'R')
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
    if (m_hexapodDesiredState != m_hexapodState || m_hexapod_desired_height != m_hexapod_height)
    {
        m_hexapod_height = m_hexapod_desired_height;

        // for some states HOME & Sleep just do the change no danger
        if (m_hexapodState == HexapodState::HOME)
        {
            m_hexapodState = m_hexapodDesiredState;
            m_walkCycleCount = 0;
        }

        if (m_hexapodState == HexapodState::SLEEP)
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
            }
        }
    }

    switch (m_hexapodState)
    {
    case HexapodState::HOME:
        MoveKeyFrame(m_poses.Home(m_hexapod_height));
        break;
    case HexapodState::SLEEP:
        MoveKeyFrame(m_poses.Sleep);
        break;

    case HexapodState::WALKFORWARD:
        switch (m_walkCycleCount)
        {
        case 0:
            MoveKeyFrame(m_poses.W0(m_hexapod_height));
            break;
        case 1:
            MoveKeyFrame(m_poses.W1(m_hexapod_height));
            break;
        case 2:
            MoveKeyFrame(m_poses.W2(m_hexapod_height));
            break;
        case 3:
            MoveKeyFrame(m_poses.W3(m_hexapod_height));
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
            MoveKeyFrame(m_poses.W0(m_hexapod_height));
            break;
        case 1:
            MoveKeyFrame(m_poses.W3(m_hexapod_height));
            break;
        case 2:
            MoveKeyFrame(m_poses.W2(m_hexapod_height));
            break;
        case 3:
            MoveKeyFrame(m_poses.W1(m_hexapod_height));
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
    // Serial.println("SetServo");

    // TODO: Convert servo speed (0-100) to maestro speed
    m_maestro.setSpeed(servo.getPololuChannel(), servo.getTargetSpeed());
    m_maestro.setAcceleration(servo.getPololuChannel(), SERVO_ACCELERATION);
    uint16_t pololuPulseWidthTarget = servo.getTargetPosition() * 4;
    m_maestro.setTarget(servo.getPololuChannel(), pololuPulseWidthTarget);
}

/*
 * Function:  Read bluetooth command
 * --------------------
 * Read bluetooth command from the HC-06 one character at a time
 * Identify a comand starting with '{' and ending with '}'
 *
 * I have included this so you can see how to do advanced serial communication
 * with a robot
 * Static variables are preserved between calls
 */
void ReadBluetoothCommand(byte &action, byte &actionValue)
{
    const char startMarker = '[';
    const char endMarker = ']';
    const byte bufferLength = 2;
    char c;
    static byte index = 0;
    static boolean recieveInProgress = false;
    static char buffer[bufferLength];

    action = ACTION_NOTHING;
    actionValue = 0;

    if (m_bluetoothSerial.available())
    {
        c = m_bluetoothSerial.read();

        // If we are starting a new command
        if (c == startMarker)
        {
            recieveInProgress = true;
            index = 0;
            return;
        }

        if (true == recieveInProgress)
        {
            // If we recieve end marker
            if (c == endMarker && index == bufferLength)
            {
                recieveInProgress = false;
                index = 0;
                action = buffer[0];
                actionValue = buffer[1] - '0';
            }
            else
            {
                if (index >= bufferLength)
                {
                    index = bufferLength - 1;
                }
                buffer[index] = c;
                index++;
            }
        }
    }
}