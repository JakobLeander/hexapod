using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class DO : MonoBehaviour {
    private DebugWindow _debugWindow;
    private BluetoothManager _bluetoothManager;
    public Button _bluetoothButton;
    private float repeatFrequency = 0.1f;




    void Start() {
        _debugWindow = gameObject.GetComponent<DebugWindow>();
        _bluetoothManager = gameObject.GetComponent<BluetoothManager>();
    }

    // Update is called once per frame
    void Update() {
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Disconnected) {
            _bluetoothButton.image.color = Color.red;
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connecting) {
            _bluetoothButton.image.color = Color.yellow;
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected) {
            _bluetoothButton.image.color = Color.white;
        }
    }

    public void DoExitApplication() {
        Application.Quit();
    }

    public void CommandButtonHandler(string command) {
        _debugWindow.LogButtonPress("Button Press: " + command);

        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected) {
            _bluetoothManager.sendData(command);
        }
    }

    public void RepeaterButtonDown(string command) {
        _debugWindow.LogButtonPress("RepeatButton Press: " + command);
        string repeatCmd = "Click" + command;
        InvokeRepeating(repeatCmd, 0, repeatFrequency);
    }
    public void RepeaterButtonUp(string command) {
        _debugWindow.LogButtonPress("RepeatButton Release: " + command);
        string repeatCmd = "Click" + command;
        CancelInvoke(repeatCmd);
    }

    // Below are used by repeaters, do not remove
    private void ClickN() {
        CommandButtonHandler("N");
    }
    private void ClickS() {
        CommandButtonHandler("S");
    }
    private void ClickE() {
        CommandButtonHandler("E");
    }
    private void ClickW() {
        CommandButtonHandler("W");
    }
    private void ClickU() {
        CommandButtonHandler("U");
    }
    private void ClickD() {
        CommandButtonHandler("D");
    }
    private void ClickL() {
        CommandButtonHandler("L");
    }
    private void ClickR() {
        CommandButtonHandler("R");
    }
    private void ClickF() {
        CommandButtonHandler("F");
    }
    private void ClickB() {
        CommandButtonHandler("B");
    }
    private void Click1() {
        CommandButtonHandler("1");
    }
    private void Click2() {
        CommandButtonHandler("2");
    }

    public void ConnectButtonHandler() {
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Disconnected) {
            _debugWindow.LogButtonPress("Connect");
            _bluetoothManager.Connect();
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected) {
            _debugWindow.LogButtonPress("Disconnect");
            _bluetoothManager.Disconnect();
        }
    }
}
