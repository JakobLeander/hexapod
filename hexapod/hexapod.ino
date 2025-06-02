#include "Servo.h"
#include "Leg.h"

#include <PololuMaestro.h> // https://www.pololu.com/docs/0J40/5.e
#include <SoftwareSerial.h>
#define RX_PIN 10                       // Pin that is connected to Maestro TX pin
#define TX_PIN 11                       // Pin that is connected to Maestro RX pin
const uint16_t SERVO_ACCELERATION = 10; // Unit is 1/4 microsecond

SoftwareSerial m_MaestroSerial(RX_PIN, TX_PIN);
MiniMaestro m_maestro(m_MaestroSerial);

Leg m_LMiddle(LegId::LMIDDLE, 0, 80, 90.0);

uint16_t ROBOT_HEIGHT = 100; // Desired height of the robot body above floor in millimeters

void setup()
{
    Serial.begin(9600);
    m_MaestroSerial.begin(9600);
    delay(1000);
    Serial.println("Ready!");

    m_LMiddle.setFootPosition(0, 80, -ROBOT_HEIGHT); // Set foot position for middle leg
}

void loop()
{
    delay(3000);
}

/// <summary>
/// Write values to a single servo
/// </summary>
/// <param name="srv">Servo to write</param>
void CommitServo(String id, uint8_t pololuChannel, uint8_t speed, uint16_t pulseWidth, float angle)
{
    // during testing print values instead
    Serial.print(id + ":");
    Serial.print(" CH=" + pololuChannel);
    Serial.print(" SPD=" + speed);
    Serial.print(" ANG=");
    Serial.print(angle);
    Serial.println(" PW=" + pulseWidth);

    // m_maestro.setSpeed(servo.getPololuChannel, m_speed);
    // m_maestro.setAcceleration(servo.getPololuChannel, SERVO_ACCELERATION);
    // m_maestro.setT arget(servo.getPololuChannel, servo.getTarget());
}