using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using System;
using System.Text;

public class LedOnOff : MonoBehaviour
{

    // Use this for initialization
    BluetoothHelper bluetoothHelper;
    string deviceName;

    string received_message;

    private string tmp;

    void Start()
    {
        deviceName = "HC-05"; //bluetooth should be turned ON;
        try
        {
            BluetoothHelper.BLE = false;
            bluetoothHelper = BluetoothHelper.GetInstance(deviceName);
            bluetoothHelper.OnConnected += OnConnected;
            bluetoothHelper.OnConnectionFailed += OnConnectionFailed;
            bluetoothHelper.OnDataReceived += OnMessageReceived; //read the data
            bluetoothHelper.OnScanEnded += OnScanEnded;

            bluetoothHelper.setTerminatorBasedStream("\n");

            if(!bluetoothHelper.ScanNearbyDevices())
            {
                //scan didnt start (on windows desktop (not UWP))
                //try to connect
                bluetoothHelper.Connect();//this will work only for bluetooth classic.
                //scanning is mandatory before connecting for BLE.

            }
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
            write(ex.Message);
        }
    }

    private void write(string msg)
    {
        tmp += ">"+ msg + "\n";
    }

    void OnMessageReceived(BluetoothHelper helper)
    {
        received_message = helper.Read();
        Debug.Log(received_message);
        write("Received : " +received_message);
    }

    void OnConnected(BluetoothHelper helper)
    {
        try
        {
            helper.StartListening();
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
            write(ex.Message);

        }
    }

    void OnScanEnded(BluetoothHelper helper, LinkedList<BluetoothDevice> devices){

        if(helper.isDevicePaired()) //we did found our device (with BLE) or we already paired the device (for Bluetooth Classic)
            helper.Connect();
        else
            helper.ScanNearbyDevices(); //we didn't
    }

    void OnConnectionFailed(BluetoothHelper helper)
    {
        write("Connection Failed");
        Debug.Log("Connection Failed");
    }


    //Call this function to emulate message receiving from bluetooth while debugging on your PC.
    void OnGUI()
    {
        tmp = GUI.TextField(new Rect(Screen.width / 4, Screen.height / 10, Screen.width / 2, Screen.height / 10 - 10), tmp);

        if (bluetoothHelper != null)
            bluetoothHelper.DrawGUI();
        else
            return;

        if (!bluetoothHelper.isConnected())
            if (GUI.Button(new Rect(Screen.width / 2 - Screen.width / 10, Screen.height / 10, Screen.width / 5, Screen.height / 10), "Connect"))
            {
                if (bluetoothHelper.isDevicePaired())
                    bluetoothHelper.Connect(); // tries to connect
                else
                    write("Cannot connect, device is not found, for bluetooth classic, pair the device\n\tFor BLE scan for nearby devices");
            }  

        if (bluetoothHelper.isConnected())
        {
            if (GUI.Button(new Rect(Screen.width / 2 - Screen.width / 10, Screen.height - 2 * Screen.height / 10, Screen.width / 5, Screen.height / 10), "Disconnect"))
            {
                bluetoothHelper.Disconnect();
                write("Disconnected");
            }

            if (GUI.Button(new Rect(Screen.width / 2 - Screen.width / 10, Screen.height / 10, Screen.width / 5, Screen.height / 10), "Turn On"))
            {
                bluetoothHelper.SendData("O");
                write("Sending O");
            }

            if (GUI.Button(new Rect(Screen.width / 2 + Screen.width / 10, Screen.height / 10, Screen.width / 5, Screen.height / 10), "Turn Off"))
            {
                bluetoothHelper.SendData("F");
                write("Sending F");
            }
            
        }
            
    }

    void OnDestroy()
    {
        if (bluetoothHelper != null)
            bluetoothHelper.Disconnect();
    }
}
