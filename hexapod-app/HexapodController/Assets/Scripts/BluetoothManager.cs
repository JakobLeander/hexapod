using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using UnityEngine.UI;

public class BluetoothManager : MonoBehaviour
{

    public enum BluetoothState { Disconnected, Connecting, Connected };
    private BluetoothHelper _btHelper;
    private BluetoothState _state;
    private DebugWindow _debugWindow;
    private const string BLUETOOTHNAME = "HC-06";


    void Start()
    {
        BluetoothHelper.BLE = false;
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
        string msg = _btHelper.Read();
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

    public void Disconnect()
    {
        _state = BluetoothState.Disconnected;
        _btHelper.Disconnect();
        _debugWindow.LogBluetooth("Disconnected");
    }

    public void sendData(string d)
    {
        // add start and end marker to the package
        // So we can safely decode on arduino
        _btHelper.SendData("[" + d +  "]");
    }
}