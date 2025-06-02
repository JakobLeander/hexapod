using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;

class MyStreamManager : ArduinoBluetoothAPI.StreamManager
{
    public override byte[] formatDataToSend(byte[] buff)
    {
        Debug.Log("Formatting data before sending " + System.Text.Encoding.ASCII.GetString(buff));
        return buff;
    }

    public override void handleReceivedData(byte[] buff)
    {
        Debug.Log("Received data " + System.Text.Encoding.ASCII.GetString(buff));
        this.OnDataReceived.Invoke(buff); //Invoke the OnDataReceived method
    }
}