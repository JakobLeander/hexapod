#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

void sendBT(const char *data, int l)
{
    char *tx_data = new byte[l + 4];
    tx_data[0] = 85; //preamble
    tx_data[1] = 85; //preamble
    tx_data[2] = (l >> 8) & 0x000000FF;
    tx_data[3] = (l & 0x000000FF);
    memcpy(tx_data+4, data, l);
    BTSerial.write(tx_data, l + 4);
    BTSerial.flush();
    delete [] tx_data;
}

void setup()
{
	BTSerial.begin(9600);
}

char *data;
int data_length;
int i = 0;
void loop()
{
	if (BTSerial.available() > 2)
	{
		data_length = 0;
		char p1 = BTSerial.read();
		char p2 = BTSerial.read();
		if(p1 != 85 || p2 != 85) return;
		while(BTSerial.available() < 2) continue;
		char x1 = BTSerial.read();
		char x2 = BTSerial.read();
		data_length = x1 << 8 | x2;
		data = new char[data_length];
		i = 0;
		while (i < data_length)
		{
			if (BTSerial.available() == 0)
				continue;

			data[i++] = BTSerial.read();
		}

		sendBT(data, data_length);

		delete[] data;
	}

	delay(100);
	sendBT("HELLOO", 6);
}

//PS char or byte ranges are -127 to 128, it you want to use 0 255 range for your binary data, simply cast it as unisgned char
// example:
// byte x = 129;
// if(x == 129) will return false
// if((unsigned char) x == 129 ) will return true
