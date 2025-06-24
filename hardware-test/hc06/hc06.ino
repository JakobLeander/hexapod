/*
  Test communication protocol for bluetooth

  By using this sketch you can validate communication between App and Arduino

  2025-06-17 Jakob Leander
*/

#include <Servo.h>

#define m_bluetoothSerial Serial1

// Global variables
unsigned long m_lastCommand;           // Last time a command was received
const unsigned long BREAK_TIME = 5000; // Number of miliseconds to allow no commands before we break
const char ACTION_NOTHING = 'N';       // Drive command

// Setup ME6
void setup()
{
    Serial.begin(115200);
    m_bluetoothSerial.begin(9600);
    m_lastCommand = millis();
    Serial.println("Ready");
    // m_bluetoothSerial.write("Ready");
}

// Main loop
void loop()
{
    byte action = ACTION_NOTHING;
    byte actionValue = 0;

    // If no request to move for a while, stop moving
    if ((millis() - m_lastCommand) > BREAK_TIME)
    {
        Serial.println("No commands: Breaking");
        m_lastCommand = millis();
    }

    // See if there are any new bluetooth commands
    ReadBluetoothCommand(action, actionValue);

    // If no action
    if (action == ACTION_NOTHING)
    {
        return;
    }
    else
    {
        Serial.print("BTCMD: ");
        Serial.print(char(action));
        Serial.print("[");
        Serial.print(actionValue);
        Serial.println("]");
        m_lastCommand = millis();
    }
}

/*
 * Function:  Read bluetooth command
 * --------------------
 * Read bluetooth command from the HC-06 one character at a time
 * Identify a comand starting with '{' and ending with '}'
 *
 * I have included this so you can see how to do advanced serial communication
 * with a robot
 * Static variables are preserved between calls
 */
void ReadBluetoothCommand(byte &action, byte &actionValue)
{
    const char startMarker = '[';
    const char endMarker = ']';
    const byte bufferLength = 2;
    char c;
    static byte index = 0;
    static boolean recieveInProgress = false;
    static char buffer[bufferLength];

    action = ACTION_NOTHING;
    actionValue = 0;

    if (m_bluetoothSerial.available())
    {
        c = m_bluetoothSerial.read();

        // If we are starting a new command
        if (c == startMarker)
        {
            recieveInProgress = true;
            index = 0;
            return;
        }

        if (true == recieveInProgress)
        {
            // If we recieve end marker
            if (c == endMarker && index == bufferLength)
            {
                recieveInProgress = false;
                index = 0;
                action = buffer[0];
                actionValue = buffer[1] - '0';
            }
            else
            {
                if (index >= bufferLength)
                {
                    index = bufferLength - 1;
                }
                buffer[index] = c;
                index++;
            }
        }
    }
}