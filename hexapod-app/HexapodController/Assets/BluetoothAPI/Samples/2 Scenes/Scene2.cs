using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using UnityEngine.SceneManagement;

public class Scene2 : MonoBehaviour
{
    private BluetoothHelper helper;
    void Start()
    {
        helper = BluetoothHelper.GetInstance(); //this will return the instance already initialized and connected (Singleton) 
        // this will return the 1st bluetoothHelperInstance initialized
        //OR
        helper = BluetoothHelper.GetInstanceById(Scene1.bluetoothHelperId);
        helper.OnDataReceived += OnDataReceived;
        helper.OnConnectionFailed += OnConnectionFailed;
        helper.StartListening();
    }

    void OnDataReceived(BluetoothHelper h)
    {
        string data = h.Read();
    }

    void OnConnectionFailed(BluetoothHelper helper)
    {
        Debug.Log("Disconnected");
        SceneManager.LoadScene("Scene1");
    }

    void OnDestroy()
    {
        helper.OnDataReceived -= OnDataReceived;
        helper.OnConnectionFailed -= OnConnectionFailed;
        helper.Disconnect();
    }
}
