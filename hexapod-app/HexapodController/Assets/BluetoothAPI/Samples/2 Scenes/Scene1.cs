using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using UnityEngine.SceneManagement;

public class Scene1 : MonoBehaviour
{
    public static int bluetoothHelperId;
    private BluetoothHelper helper;
    void Start()
    {
        helper = BluetoothHelper.GetNewInstance("HC-05");
        bluetoothHelperId = helper.getId();
        helper.OnConnected += OnConnected;
        helper.OnConnectionFailed += OnConnectionFailed;
        helper.setTerminatorBasedStream("\n");
        helper.Connect();
    }

    void OnConnected(BluetoothHelper helper)
    {
        SceneManager.LoadScene("Scene2");
    }

    void OnConnectionFailed(BluetoothHelper helper)
    {
        Debug.Log("Failed to connect, retrying");
        helper.Connect();
    }


    void OnDestroy()
    {
        helper.OnConnected -= OnConnected;
        helper.OnConnectionFailed -= OnConnectionFailed;
    }


}
