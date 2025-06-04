using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Hexapod : MonoBehaviour
{
    public Button _bluetoothButton;
    public FixedJoystick _moveJoystick;
    public FixedJoystick _positionJoystick;
    public Slider _heightSlider;
    public Text _moveJoystickText;
    public Text _positionJoystickText;

    private DebugWindow _debugWindow;
    private BluetoothManager _bluetoothManager;
    private float repeatFrequency = 0.1f;
    private const int TIMES_PER_SECOND_TO_SUBMIT_JOYSTICK_DATA = 4;
    private int _submitJoystickDataCounter = 50 / TIMES_PER_SECOND_TO_SUBMIT_JOYSTICK_DATA;


    void Start()
    {
        _debugWindow = gameObject.GetComponent<DebugWindow>();
        _bluetoothManager = gameObject.GetComponent<BluetoothManager>();
    }

    // Update is called once per frame
    void Update()
    {
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Disconnected)
        {
            _bluetoothButton.image.color = Color.red;
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connecting)
        {
            _bluetoothButton.image.color = Color.yellow;
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected)
        {
            _bluetoothButton.image.color = Color.white;
        }

        // Show joystick values
       _moveJoystickText.text = "" + _moveJoystick.Direction + "";
       _positionJoystickText.text = "" + _positionJoystick.Direction + "";
    }

    //  Called 50 times per second
    private void FixedUpdate()
    {
        if (_submitJoystickDataCounter <= 0)
        {
            int moveX = (int)(_moveJoystick.Direction.x * 100);
            int moveY = (int)(_moveJoystick.Direction.y * 100);
            int positionX = (int)(_positionJoystick.Direction.x * 100);
            int positionY = (int)(_positionJoystick.Direction.y * 100);

            // only send data if joystick are moved
            if (moveX!=0 || moveY!=0||positionX!=0||positionY!=0){
                string command = string.Format("J{0},{1},{2},{3}",moveX,moveY,positionX,positionY);
                _debugWindow.LogButtonPress(command);

                SendBluetoothData(command);
            }

            _submitJoystickDataCounter = 50 / TIMES_PER_SECOND_TO_SUBMIT_JOYSTICK_DATA;
        }
        _submitJoystickDataCounter--;
    }

    public void DoExitApplication()
    {
        Application.Quit();
    }

    public void HeightSliderChanged()
    {
        int height = (int)(Mathf.Clamp(_heightSlider.value,0,100));
        _debugWindow.LogButtonPress("Height: " + height);
        SendBluetoothData("H" + height);
    }

    public void CommandButtonHandler(string command)
    {
        _debugWindow.LogButtonPress("Button Press: " + command);
        SendBluetoothData(command);
    }

    public void SleepButtonHandler()
    {
        _debugWindow.LogButtonPress("Button Press: Sleep");

        SendBluetoothData("S");
        
    }

    private void SendBluetoothData(string data )
    {
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected)
        {
            _bluetoothManager.sendData(data);
        }
    }


    public void ConnectButtonHandler()
    {
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Disconnected)
        {
            _debugWindow.LogButtonPress("Connect");
            _bluetoothManager.Connect();
        }
        if (_bluetoothManager.GetState() == BluetoothManager.BluetoothState.Connected)
        {
            _debugWindow.LogButtonPress("Disconnect");
            _bluetoothManager.Disconnect();
        }
    }
}
