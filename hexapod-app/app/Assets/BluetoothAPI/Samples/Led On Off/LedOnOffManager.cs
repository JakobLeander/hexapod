using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;

public class LedOnOffManager : MonoBehaviour
{
    private BluetoothHelper helper;
    public GameObject connectBtn;
    public GameObject disconnectBtn;
    public GameObject led;

    void Start()
    {
        BluetoothHelper.BLE = false;
        helper = BluetoothHelper.GetInstance();
        helper.OnConnected += OnConnected;
        helper.OnConnectionFailed += OnConnectionFailed;
        helper.OnDataReceived += OnDataReceived;
        helper.setFixedLengthBasedStream(1); //data is received byte by byte
        helper.setDeviceName("HC-05");
    }

    void OnConnected(BluetoothHelper helper)
    {
        helper.StartListening();
        connectBtn.SetActive(false);
        disconnectBtn.SetActive(true);
    }

    void OnConnectionFailed(BluetoothHelper helper)
    {
        Debug.Log("Failed to connect");
        connectBtn.SetActive(true);
        disconnectBtn.SetActive(false);
    }

    void OnDataReceived(BluetoothHelper helper)
    {
        string msg = helper.Read();

        switch(msg)
        {
            case "1":
                led.GetComponent<Renderer>().material.color = Color.red;
                break;
            case "0":
                led.GetComponent<Renderer>().material.color = Color.gray;
                break;
            default:
                Debug.Log($"Received unknown message [{msg}]");
                break;
        }
    }

    public void Connect()
    {
        helper.Connect();
    }

    public void Disconnect()
    {
        helper.Disconnect();
        connectBtn.SetActive(true);
        disconnectBtn.SetActive(false);
    }

    public void sendData(string d)
    {
        helper.SendData(d);
    }
    public void sendTestData()
    {
        helper.SendData("TEST");
    }
}
