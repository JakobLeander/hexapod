//Connect TX pin of the HC-05 to RX pin of the Arduino
//Connect RX pin of the HC-05 to TX pin of the Arduino
//You can use SoftwareSerial Library, but i dont recommend it for fast and long data transmission
//Otherwise you have to check Serial.available() > excepted number of bytes sent before reading the message
//There's no problem with hardware serial that comes with the arduino. It's perfect
//
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void readBT();
void sendBT(const char *data, int l)
{
    char *tx_data = new byte[l + 4];
    tx_data[0] = 85; //preamble
    tx_data[1] = 85; //preamble
    tx_data[2] = (l >> 8) & 0x000000FF;
    tx_data[3] = (l & 0x000000FF);
    memcpy(tx_data + 4, data, l);
    BTSerial.write(tx_data, l + 4);
    BTSerial.flush();
    delete[] tx_data;
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  mySerial.begin(9600);
  Serial.begin(9600);
  
    digitalWrite(2, HIGH); 
    delay(1000);
    digitalWrite(2, LOW); 
  }

char* data;
int data_length;
int i=0;
int timeout;
int highLow;
char* send_data;

void loop() {
  readBT();
}


void readBT()
{
  if(mySerial.available() >= 2)
  {
    timeout=0;
    data_length = 0;
    byte pre1 = mySerial.read();
    byte pre2 = mySerial.read();
    if(pre1 != 85 || pre2 != 85) return;
    while(mySerial.available() < 2) continue;
    byte x1 = mySerial.read();
    byte x2 = mySerial.read();

    data_length = x1 << 8 | x2;
    
    data = new char[data_length];
    i=0;
    while(i<data_length)
    {
        
      if(mySerial.available()==0){
        if(++timeout == 1000)
          goto FreeData;
        continue;
      }
      timeout=0;
      data[i++] = mySerial.read();
    }

    send_data = new char[data_length+1];
    send_data[0] = 'S';
    for(byte i=0; i<2;i++)
      send_data[i+1] = data[i];

    highLow = data[0] == 'E' ? HIGH : LOW;

    digitalWrite(data[1], highLow);

    sendBT(send_data, data_length+1);
    delete [] send_data;

    FreeData:
    delete[] data;
  }
}