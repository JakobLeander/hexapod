#include <SoftwareSerial.h>

#define RX  10
#define TX  11
#define LED 5


SoftwareSerial mySerial(RX, TX);


char data;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  data = 0;
}

void loop()
{
  if (mySerial.available())
  {
    // data received from UNITY
    data = mySerial.read();

    switch(data)
    {
      case 'Y':
        digitalWrite(LED, HIGH);
        mySerial.print('1'); //send feedback to unity
        break;
      case 'N':
        digitalWrite(LED, LOW);
        mySerial.print('0');  //send feedback to unity
        break;
    }
  }
}
