using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using UnityEngine.UI;
using System;

public class BluetoothManager : MonoBehaviour
{

    public enum BluetoothState { Disconnected, Connecting, Connected };
    private int _batteryLevel;
    private BluetoothHelper _btHelper;
    private BluetoothState _state;
    private DebugWindow _debugWindow;
    private const string BLUETOOTHNAME = "HC-06";


    void Start()
    {
        BluetoothHelper.BLE = false;
        _batteryLevel = 1;
        _btHelper = BluetoothHelper.GetInstance();
        _btHelper.OnConnected += OnConnected;
        _btHelper.OnConnectionFailed += OnConnectionFailed;
        _btHelper.OnDataReceived += OnDataReceived;
        _btHelper.setFixedLengthBasedStream(1); //data is received byte by byte
        _btHelper.setDeviceName(BLUETOOTHNAME);
        _state = BluetoothState.Disconnected;
        _debugWindow = gameObject.GetComponent<DebugWindow>();
        _debugWindow.LogBluetooth("Awaiting connection to: " + BLUETOOTHNAME);

    }

    void OnConnected(BluetoothHelper helper)
    {
        _state = BluetoothState.Connected;
        _debugWindow.LogBluetooth("Connected");
        _btHelper.StartListening();
    }


    void OnConnectionFailed(BluetoothHelper helper)
    {
        _state = BluetoothState.Disconnected;
        _debugWindow.LogBluetooth("Disconnected");
    }

    void OnDataReceived(BluetoothHelper helper)
    {
        string data= helper.Read();

        if (Int32.TryParse(data, out int battery))
        {
            if (battery >= 0 && battery <= 9)
            {
                _batteryLevel = battery;
            }
        }
    }

    public void Connect()
    {
        _state = BluetoothState.Connecting;
        _debugWindow.LogBluetooth("Connecting");
        _btHelper.Connect();
    }

    public BluetoothState GetState()
    {
        return _state;
    }

    public int GetBatteryLevel()
    {
        return _batteryLevel;
    }

    public void Disconnect()
    {
        _state = BluetoothState.Disconnected;
        _btHelper.Disconnect();
        _debugWindow.LogBluetooth("Disconnected");
    }

    public void sendData(string data)
    {
        // add start and end marker to the package
        // So we can safely decode on arduino
        _btHelper.SendData("[" + data +  "]");
    }
}