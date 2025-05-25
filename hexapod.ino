#include "Servo.h"
#include "Keyframes.h"
#include <PololuMaestro.h>
// https://www.pololu.com/docs/0J40/5.e
#include <SoftwareSerial.h>
#define RX_PIN 10                       // Pin that is connected to Maestro TX pin
#define TX_PIN 11                       // Pin that is connected to Maestro RX pin
const uint16_t SERVO_ACCELERATION = 10; // Unit is 1/4 microsecond

// Initialize servos
Servo m_LBFootServo{"LBFoot", 0, true};
Servo m_LBKneeServo{"LBKnee", 1, false};
Servo m_LBHipServo{"LBHip", 2, false};
Servo m_LMFootServo{"LMFoot", 3, true};
Servo m_LMKneeServo{"LMKnee", 4, false};
Servo m_LMHipServo{"LMHip", 5, false};
Servo m_LFFootServo{"LFFoot", 6, true};
Servo m_LFKneeServo{"LFKnee", 7, false};
Servo m_LFHipServo{"LFHip", 8, false};
Servo m_RFFootServo{"RFFoot", 9, false};
Servo m_RFKneeServo{"RFKnee", 10, true};
Servo m_RFHipServo{"RFHip", 11, true};
Servo m_RMFootServo{"RMFoot", 12, false};
Servo m_RMKneeServo{"RMKnee", 13, true};
Servo m_RMHipServo{"RMHip", 14, true};
Servo m_RBFootServo{"RBFoot", 15, false};
Servo m_RBKneeServo{"RBKnee", 16, true};
Servo m_RBHipServo{"RBHip", 17, true};

Servo my_servo{"test", 0, false};

KeyFrames m_KeyFrames;
SoftwareSerial m_MaestroSerial(RX_PIN, TX_PIN);
MiniMaestro m_maestro(m_MaestroSerial);

uint16_t m_speed = 10; // 0 means unlimited speed

void setup()
{
    Serial.begin(9600);
    m_MaestroSerial.begin(9600);
    delay(1000);
    // SFrame homePose = keyFrames.HomePose;
    CommitAllServos();
    Serial.println("Ready!");
}

void loop()
{
    Serial.println(my_servo.getId());
    uint8_t channel = my_servo.getPololuChannel();
    delay(3000);
}

void CommitAllServos()
{
    CommitServo(m_LBFootServo);
    CommitServo(m_LBKneeServo);
    CommitServo(m_LBHipServo);
    CommitServo(m_LMFootServo);
    CommitServo(m_LMKneeServo);
    CommitServo(m_LMHipServo);
    CommitServo(m_LFFootServo);
    CommitServo(m_LFKneeServo);
    CommitServo(m_LFHipServo);
    CommitServo(m_RBFootServo);
    CommitServo(m_RBKneeServo);
    CommitServo(m_RBHipServo);
    CommitServo(m_RMFootServo);
    CommitServo(m_RMKneeServo);
    CommitServo(m_RMHipServo);
    CommitServo(m_RFFootServo);
    CommitServo(m_RFKneeServo);
    CommitServo(m_RFHipServo);
}

/// <summary>
/// Write values to a single servo
/// </summary>
/// <param name="srv">Servo to write</param>
void CommitServo(Servo servo)
{
    // during testing print values instead
    Serial.print(servo.getId() + ": ");
    Serial.print(servo.getPololuChannel());
    Serial.print(", ");
    Serial.print(servo.getTarget());
    Serial.println(")");

    // m_maestro.setSpeed(servo.getPololuChannel, m_speed);
    // m_maestro.setAcceleration(servo.getPololuChannel, SERVO_ACCELERATION);
    // m_maestro.setT arget(servo.getPololuChannel, servo.getTarget());
}