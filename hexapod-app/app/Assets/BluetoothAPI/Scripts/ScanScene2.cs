using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using System;
using UnityEngine.UI;

public class ScanScene2 : MonoBehaviour
{

    // Use this for initialization
    BluetoothHelper bluetoothHelper;
    BluetoothHelper bluetoothHelper2;

    public Text text;
    public GameObject sphere;

    string received_message;

    void Start()
    {
        try
        {
            BluetoothHelper.BLE = true;  //use Bluetooth Low Energy Technology
            bluetoothHelper = BluetoothHelper.GetInstance();

            bluetoothHelper.OnConnected += OnConnected;
            bluetoothHelper.OnConnectionFailed += OnConnectionFailed;
            bluetoothHelper.OnDataReceived += OnMessageReceived; //read the data
            bluetoothHelper.OnScanEnded += OnScanEnded;

            bluetoothHelper.setTerminatorBasedStream("\n");

            bluetoothHelper2 = BluetoothHelper.GetNewInstance();
            bluetoothHelper2.OnConnected += OnConnected;
            bluetoothHelper2.OnConnectionFailed += OnConnectionFailed;
            bluetoothHelper2.OnScanEnded += OnScanEnded2;
            bluetoothHelper2.OnCharacteristicChanged += (helper, value, characteristic) =>
           {
               Debug.Log(characteristic.getName());
               Debug.Log(System.Text.Encoding.ASCII.GetString(value));
           };


            BluetoothHelperService service = new BluetoothHelperService("180D");
            service.addCharacteristic(new BluetoothHelperCharacteristic("2A37"));
            service.addCharacteristic(new BluetoothHelperCharacteristic("2A38"));
            service.addCharacteristic(new BluetoothHelperCharacteristic("2A39"));
            bluetoothHelper2.Subscribe(service);


            bluetoothHelper.ScanNearbyDevices();



            text.text = "start scan";

        }
        catch (BluetoothHelper.BlueToothNotEnabledException ex)
        {
            sphere.GetComponent<Renderer>().material.color = Color.yellow;
            Debug.Log(ex.ToString());
            text.text = ex.Message;
        }
    }

    IEnumerator blinkSphere()
    {
        sphere.GetComponent<Renderer>().material.color = Color.cyan;
        yield return new WaitForSeconds(0.5f);
        sphere.GetComponent<Renderer>().material.color = Color.green;
    }

    //Asynchronous method to receive messages
    void OnMessageReceived(BluetoothHelper helper)
    {
        //StartCoroutine(blinkSphere());
        received_message = helper.Read();
        text.text = received_message;
        Debug.Log(System.DateTime.Now.Second);
        //Debug.Log(received_message);
    }

    void OnScanEnded(BluetoothHelper helper, LinkedList<BluetoothDevice> nearbyDevices)
    {
        Debug.Log("1 ended");

        if (nearbyDevices.Count == 0)
        {
            helper.ScanNearbyDevices();
            return;
        }


        foreach (BluetoothDevice device in nearbyDevices)
        {
            if (device.DeviceName == "HC-08")
                Debug.Log("FOUND!!");
        }

        text.text = "HC-08";
        bluetoothHelper2.setDeviceName("HC-08");
        // bluetoothHelper.setDeviceAddress("00:21:13:02:16:B1");
        bluetoothHelper2.Connect();
        bluetoothHelper2.isDevicePaired();

    }

    void OnScanEnded2(BluetoothHelper helper, LinkedList<BluetoothDevice> nearbyDevices)
    {
        Debug.Log("2 ended " + nearbyDevices.Count);
        if (nearbyDevices.Count == 0)
        {
            helper.ScanNearbyDevices();
            return;
        }


        foreach (BluetoothDevice device in nearbyDevices)
        {
            Debug.Log(device.DeviceName);
            if (device.DeviceName == "HUAWEI Y7 Prime 2018")
                Debug.Log("FOUND!!");
        }


        helper.setDeviceName("HUAWEI Y7 Prime 2018");
        helper.Connect();
    }

    void Update()
    {
        //Debug.Log(bluetoothHelper.IsBluetoothEnabled());
        if (!bluetoothHelper.IsBluetoothEnabled())
        {
            bluetoothHelper.EnableBluetooth(true);
        }
    }

    void OnConnected(BluetoothHelper helper)
    {
        sphere.GetComponent<Renderer>().material.color = Color.green;
        try
        {
            helper.StartListening();
            if(helper.getId() == bluetoothHelper.getId()) //1st instance connected, connect the second
                bluetoothHelper2.ScanNearbyDevices();
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
        }

    }
    void OnConnectionFailed(BluetoothHelper helper)
    {
        sphere.GetComponent<Renderer>().material.color = Color.red;
        Debug.Log("Connection Failed");
    }

    //Call this function to emulate message receiving from bluetooth while debugging on your PC.
    void OnGUI()
    {
        if (bluetoothHelper != null)
            bluetoothHelper.DrawGUI();
        else
            return;

        if (bluetoothHelper.isConnected())
            if (GUI.Button(new Rect(Screen.width / 2 - Screen.width / 10, Screen.height - 2 * Screen.height / 10, Screen.width / 5, Screen.height / 10), "Disconnect"))
            {
                bluetoothHelper.Disconnect();
                sphere.GetComponent<Renderer>().material.color = Color.blue;
            }

        if (bluetoothHelper.isConnected())
            if (GUI.Button(new Rect(Screen.width / 2 - Screen.width / 10, Screen.height / 10, Screen.width / 5, Screen.height / 10), "Send text"))
            {
                //bluetoothHelper.SendData(new byte[] { 0, 1, 2, 3, 4 });
                bluetoothHelper.SendData("Hi From unity");
            }
    }

    void OnDestroy()
    {
        if (bluetoothHelper != null)
            bluetoothHelper.Disconnect();
        if (bluetoothHelper2 != null)
            bluetoothHelper2.Disconnect();
    }
}
