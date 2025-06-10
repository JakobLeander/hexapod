#include "Servo.h"
#include "Leg.h"
#include "Poses.h"
#include "KeyFrame.h"
#include "Position.h"

#include <PololuMaestro.h> // https://www.pololu.com/docs/0J40/5.e
#include <SoftwareSerial.h>
#define RX_PIN 10                       // Pin that is connected to Maestro TX pin
#define TX_PIN 11                       // Pin that is connected to Maestro RX pin
const uint16_t SERVO_ACCELERATION = 0; // Unit is 1/4 microsecond
const uint16_t ROBOT_SPEED = 100;        // TODO: Make dynamic

SoftwareSerial m_MaestroSerial(RX_PIN, TX_PIN);
MiniMaestro m_maestro(m_MaestroSerial);
Poses m_poses;

// Initialize legs with their respective anchor positions
Leg m_LFront(LegId::LFRONT, 74, 39);
Leg m_LMiddle(LegId::LMIDDLE, 0, 64);
Leg m_LBack(LegId::LBACK, -74, 39);
Leg m_RFront(LegId::RFRONT, 74, -39);
Leg m_RMiddle(LegId::RMIDDLE, 0, -64);
Leg m_RBack(LegId::RBACK, -74, -39);

KeyFrame m_currentKeyFrame = {m_poses.Home};
KeyFrame m_targetKeyFrame = {m_poses.Home};
bool m_front=false;

// TODO: Make dynamic
// Since our body center is higher than joint it cannot be zero
int16_t ROBOT_HEIGHT = 127; // Desired height of the robot body above floor in millimeters
const int8_t INTERPOLATIONS=5;
int8_t m_interpolationCount = -1;

void setup()
{
    Serial.begin(115200);
    m_MaestroSerial.begin(115200);
    delay(1000);
    Serial.println("Ready!");

    // Test Servo Calculations
    ZeroAllLegs(); // Set all servos to zero angle
    delay(1000);

    // Move to Sleep position
    MoveKeyFrame(m_poses.HomeLLegForward);

    // m_RFront.setFootPosition(174, -70, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    // CommitLeg(m_RFront);

    /*
    Serial.println("");
    Serial.println("***** Foot moved forward a bit");
    m_LMiddle.setFootPosition(0 + 70, 177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_LMiddle);
    delay(1000);
    Serial.println("");
    Serial.println("***** Foot moved middle");
    m_LMiddle.setFootPosition(0 + 0, 177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_LMiddle);
    delay(1000);
    Serial.println("");
    Serial.println("***** Foot moved backward a bit");
    m_LMiddle.setFootPosition(0 - 70, 177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_LMiddle);
    delay(1000);
    */
}

void loop()
{
    // Do interpolation if we are in middle of a move
    if (m_interpolationCount >= 0 && not IsServosMoving())
    {
        Interpolate();
        //delay(1000);
    }

    if(m_interpolationCount<0){
        if(m_front){
            delay(1000);
            MoveKeyFrame(m_poses.HomeLLegForward);
            Serial.println("***** Move Forward");

            m_front=false;
        }
        else{
            delay(1000);
            MoveKeyFrame(m_poses.HomeLLegBack);
            m_front=true;
            Serial.println("***** Move Backward");
        }
    }


}

void Interpolate()
{
    //Serial.println("InterpolationCount: " + String(m_interpolationCount));
    if (m_interpolationCount < 0)
    {
        return;
    }

    float stepFactor = ((float)INTERPOLATIONS - (float)m_interpolationCount) / (float)INTERPOLATIONS; // Calculate step factor from 0 to 1
    //Serial.println("StepFactor: " + String(stepFactor));

    Position lFrontPosition = {m_currentKeyFrame.LFront.x + (m_targetKeyFrame.LFront.x - m_currentKeyFrame.LFront.x) * stepFactor,
                               m_currentKeyFrame.LFront.y + (m_targetKeyFrame.LFront.y - m_currentKeyFrame.LFront.y) * stepFactor,
                               m_currentKeyFrame.LFront.z + (m_targetKeyFrame.LFront.z - m_currentKeyFrame.LFront.z) * stepFactor};
    m_LFront.setFootPosition(lFrontPosition, ROBOT_SPEED);

    Position lMiddlePosition = {m_currentKeyFrame.LMiddle.x + (m_targetKeyFrame.LMiddle.x - m_currentKeyFrame.LMiddle.x) * stepFactor,
                                m_currentKeyFrame.LMiddle.y + (m_targetKeyFrame.LMiddle.y - m_currentKeyFrame.LMiddle.y) * stepFactor,
                                m_currentKeyFrame.LMiddle.z + (m_targetKeyFrame.LMiddle.z - m_currentKeyFrame.LMiddle.z) * stepFactor};
    m_LMiddle.setFootPosition(lMiddlePosition, ROBOT_SPEED);
    //Serial.println("LMiddle: " + String(lMiddlePosition.x) + ", " + String(lMiddlePosition.y) + ", " + String(lMiddlePosition.z));

    Position lBackPosition = {m_currentKeyFrame.LBack.x + (m_targetKeyFrame.LBack.x - m_currentKeyFrame.LBack.x) * stepFactor,
                              m_currentKeyFrame.LBack.y + (m_targetKeyFrame.LBack.y - m_currentKeyFrame.LBack.y) * stepFactor,
                              m_currentKeyFrame.LBack.z + (m_targetKeyFrame.LBack.z - m_currentKeyFrame.LBack.z) * stepFactor};
    m_LBack.setFootPosition(lBackPosition, ROBOT_SPEED);

    Position rFrontPosition = {m_currentKeyFrame.RFront.x + (m_targetKeyFrame.RFront.x - m_currentKeyFrame.RFront.x) * stepFactor,
                               m_currentKeyFrame.RFront.y + (m_targetKeyFrame.RFront.y - m_currentKeyFrame.RFront.y) * stepFactor,
                               m_currentKeyFrame.RFront.z + (m_targetKeyFrame.RFront.z - m_currentKeyFrame.RFront.z) * stepFactor};
    m_RFront.setFootPosition(rFrontPosition, ROBOT_SPEED);

    Position rMiddlePosition = {m_currentKeyFrame.RMiddle.x + (m_targetKeyFrame.RMiddle.x - m_currentKeyFrame.RMiddle.x) * stepFactor,
                                m_currentKeyFrame.RMiddle.y + (m_targetKeyFrame.RMiddle.y - m_currentKeyFrame.RMiddle.y) * stepFactor,
                                m_currentKeyFrame.RMiddle.z + (m_targetKeyFrame.RMiddle.z - m_currentKeyFrame.RMiddle.z) * stepFactor};
    m_RMiddle.setFootPosition(rMiddlePosition, ROBOT_SPEED);

    Position rBackPosition = {m_currentKeyFrame.RBack.x + (m_targetKeyFrame.RBack.x - m_currentKeyFrame.RBack.x) * stepFactor,
                              m_currentKeyFrame.RBack.y + (m_targetKeyFrame.RBack.y - m_currentKeyFrame.RBack.y) * stepFactor,
                              m_currentKeyFrame.RBack.z + (m_targetKeyFrame.RBack.z - m_currentKeyFrame.RBack.z) * stepFactor};
    m_RBack.setFootPosition(rBackPosition, ROBOT_SPEED);
    // Commit all legs to write values to servos
    CommitAllLegs();
    m_interpolationCount--;
    if (m_interpolationCount <= 0)
    {
        // Update current key frame to target key frame
        m_currentKeyFrame = m_targetKeyFrame;
    }
}

void ZeroAllLegs()
{
    // Set all servos to zero angle
    m_LFront.zeroAngles(ROBOT_SPEED);
    m_LMiddle.zeroAngles(ROBOT_SPEED);
    m_LBack.zeroAngles(ROBOT_SPEED);
    m_RFront.zeroAngles(ROBOT_SPEED);
    m_RMiddle.zeroAngles(ROBOT_SPEED);
    m_RBack.zeroAngles(ROBOT_SPEED);

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
    CommitLeg(m_LFront);
    CommitLeg(m_LMiddle);
    CommitLeg(m_LBack);
    CommitLeg(m_RFront);
    CommitLeg(m_RMiddle);
    CommitLeg(m_RBack);
}

void CommitLeg(Leg &leg)
{
    // Write values to all servos of the leg
    CommitServo(leg.getHipServo());
    CommitServo(leg.getKneeServo());
    CommitServo(leg.getFootServo());
}

void SetSpeed(){

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