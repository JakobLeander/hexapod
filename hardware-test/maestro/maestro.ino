
#include <PololuMaestro.h>
// https://www.pololu.com/docs/0J40/5.e

#define RX_PIN 10 // Pin that is connected to Maestro TX pin
#define TX_PIN 11 // Pin that is connected to Maestro RX pin

#define MAESTRO_SERVO_PIN 0 //

#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(RX_PIN, TX_PIN);

MiniMaestro maestro(maestroSerial);

const uint16_t SERVO_ACC = 127;              // Unit is 1/4 microsecond
const uint16_t SERVO_SPEED = 30;             // 0 unlimited
const uint16_t SERVO_PULSE_WIDTH_MIN = 576;  // Also set in pololu control center
const uint16_t SERVO_PULSE_WIDTH_MAX = 2464; // Also set in pololu control center
// These value gives a center of 1520 which is what my servo tester outputs

const uint16_t SERVO_STATE_MIN = SERVO_PULSE_WIDTH_MIN * 4; // pulse width to transmit in units of quarter-microseconds, 0 means stop
const uint16_t SERVO_STATE_MAX = SERVO_PULSE_WIDTH_MAX * 4; // pulse width to transmit in units of quarter-microseconds, 0 means stop

const int SERVO_MIN_ANGLE = -90;
const int SERVO_MAX_ANGLE = +90;

void setup()
{
    Serial.begin(9600);
    maestroSerial.begin(9600);
    maestro.setSpeed(0, SERVO_SPEED);
    maestro.setAcceleration(0, SERVO_ACC);
    delay(1000);
    Serial.println("Ready");
}

void loop()
{
    uint16_t pulse = 0;
    pulse = getPulse(SERVO_MIN_ANGLE);
    maestro.setTarget(0, pulse);
    Serial.print("Rotating: 1: ");
    Serial.println(pulse);
    wait();
    delay(2000);

    pulse = getPulse(SERVO_MAX_ANGLE);
    maestro.setTarget(0, pulse);
    Serial.print("Rotating: 2: ");
    Serial.println(pulse);
    wait();
    delay(2000);
}

void wait()
{
    uint8_t movingState = 255;
    while (movingState > 0)
    {
        delay(100);
        movingState = maestro.getMovingState();
    }
}

uint16_t getPulse(int angle)
{
    uint16_t pulse = map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_STATE_MIN, SERVO_STATE_MAX);
    return pulse;
}
