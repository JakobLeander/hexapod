#include "Servo.h"
#include "Leg.h"

#include <PololuMaestro.h> // https://www.pololu.com/docs/0J40/5.e
#include <SoftwareSerial.h>
#define RX_PIN 10                       // Pin that is connected to Maestro TX pin
#define TX_PIN 11                       // Pin that is connected to Maestro RX pin
const uint16_t SERVO_ACCELERATION = 10; // Unit is 1/4 microsecond
const uint16_t ROBOT_SPEED = 20;        // TODO: Make dynamic

SoftwareSerial m_MaestroSerial(RX_PIN, TX_PIN);
MiniMaestro m_maestro(m_MaestroSerial);

// Initialize legs with their respective anchor positions
Leg m_LFront(LegId::LFRONT, 74, 39);
Leg m_LMiddle(LegId::LMIDDLE, 0, 64);
Leg m_LBack(LegId::LBACK, -74, 39);
Leg m_RFront(LegId::RFRONT, 74, -39);
Leg m_RMiddle(LegId::RMIDDLE, 0, -64);
Leg m_RBack(LegId::RBACK, -74, -39);

// TODO: Make dynamic
// Since our body center is higher than joint it cannot be zero
int16_t ROBOT_HEIGHT = 127; // Desired height of the robot body above floor in millimeters

void setup()
{
    Serial.begin(115200);
    m_MaestroSerial.begin(115200);
    delay(1000);
    Serial.println("Ready!");

    // Test Servo Calculations
    ZeroAllLegs(); // Set all servos to zero angle
    delay(1000);

    HomePosition();

    //m_RFront.setFootPosition(174, -70, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    //CommitLeg(m_RFront);

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
    /*
    Serial.println("");
    Serial.println("***** Foot moved forward a bit");
    m_RMiddle.setFootPosition(0 + 70, -177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_RMiddle);
    delay(1000);
    Serial.println("");
    Serial.println("***** Foot moved backward a bit");
    m_RMiddle.setFootPosition(0 - 70, -177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_RMiddle);
    delay(1000);
    */
    /*
    Serial.println("");
    Serial.println("Foot moved up a bit");
    m_RMiddle.setFootPosition(0, -177, -ROBOT_HEIGHT+50, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_RMiddle);
    delay(1000);
    Serial.println("Foot moved middle");
    m_RMiddle.setFootPosition(0, -177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitLeg(m_RMiddle);
    delay(1000);
    */

    
    while(ROBOT_HEIGHT>50){
        ROBOT_HEIGHT = ROBOT_HEIGHT - 10;
        HomePosition();
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

void HomePosition(){
        m_LFront.setFootPosition(174, 92, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        m_LMiddle.setFootPosition(0, 177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        m_LBack.setFootPosition(-174, 92, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        m_RFront.setFootPosition(174, -92, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        m_RMiddle.setFootPosition(0, -177, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        m_RBack.setFootPosition(-174, -92, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
        CommitAllLegs();


}

void ServoTest()
{
    // Test servo movement
    Serial.println("***** Servo test started *****");
    Serial.println("***** Left Middle Leg");
    Serial.println("Foot positioned so all angles should be 0 degree");
    m_LMiddle.setFootPosition(0, 196, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                               // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved up a bit");
    m_LMiddle.setFootPosition(0, 196, -ROBOT_HEIGHT + 50, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                    // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved forward a bit");
    m_LMiddle.setFootPosition(0 + 50, 196, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                    // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved away a bit");
    m_LMiddle.setFootPosition(0, 196 + 50, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                    // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Illegal position, should not move");
    m_LMiddle.setFootPosition(0, 196 + 500, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                     // Write values to all servos
    delay(500);

    // right middle leg
    Serial.println("");
    Serial.println("***** Right Middle Leg");
    Serial.println("Foot positioned so all angles should be 0 degree");
    m_RMiddle.setFootPosition(0, -196, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved up a bit");
    m_RMiddle.setFootPosition(0, -196, -ROBOT_HEIGHT + 50, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                     // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved forward a bit");
    m_RMiddle.setFootPosition(0 + 50, -196, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                     // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Foot moved away a bit");
    m_RMiddle.setFootPosition(0, -196 - 50, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                     // Write values to all servos
    delay(500);

    Serial.println("");
    Serial.println("Illegal position, should not move");
    m_RMiddle.setFootPosition(0, 196 + 500, -ROBOT_HEIGHT, ROBOT_SPEED); // Set foot position for middle leg
    CommitAllLegs();                                                     // Write values to all servos
    delay(500);

    Serial.println("Servo test finished");
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