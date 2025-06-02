#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  mySerial.begin(9600);
  pinMode(13, OUTPUT);
}

String data = "";

void loop()
{
    if (mySerial.available()) 
    {
        data = mySerial.readStringUntil('\n');
        if(data == "O")
        {
            digitalWrite(13, HIGH);
        }else if(data == "F")
        {
            digitalWrite(13, LOW);
        }
    }

    delay(10);
}
