//Connect TX pin of the HC-05 to RX pin of the Arduino
//Connect RX pin of the HC-05 to TX pin of the Arduino
//You can use SoftwareSerial Library, but i dont recommend it for fast and long data transmission
//Otherwise you have to check Serial.available() > excepted number of bytes sent before reading the message
//There's no problem with hardware serial that comes with the arduino. It's perfect
//
void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if (Serial.available() > 0)
	{
		Serial.print("You Wrote : ");
		Serial.flush();
		while (Serial.available() > 0)
		{
			Serial.write((char)Serial.read());
			Serial.flush(); //this is a must for the arduino to wait untill Serial.write is done
		}
	}

	delay(100);
	Serial.println("Send here any text");
}
