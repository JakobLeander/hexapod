using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class Hexapod : MonoBehaviour
{
    public Button _bluetoothButton;
    public Image _batteryImage;
    public Sprite Battery01;
    public Sprite Battery02;
    public Sprite Battery03;
    public Sprite Battery04;
    public FixedJoystick _moveJoystick;
    public FixedJoystick _positionJoystick;
    public Slider _heightSlider;
    public Text _moveJoystickText;
    public Text _positionJoystickText;

    private DebugWindow _debugWindow;
    private BluetoothManager _bluetoothManager;
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

        int level = _bluetoothManager.GetBatteryLevel();
        if (level == 0 || level == 1)
        {
            _batteryImage.sprite = Battery01;
        }
        if (level == 2 || level == 3||level==4)
        {
            _batteryImage.sprite = Battery02;
        }
        if (level == 5 || level == 6 || level == 7)
        {
            _batteryImage.sprite = Battery03;
        }
        if (level==8||level==9 )
        {
            _batteryImage.sprite = Battery04;
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
            // Move is a value from 0 to 9
            int moveY = (int)(_moveJoystick.Direction.y * 9);
            int positionX = (int)(_positionJoystick.Direction.x * 9);
            string command = "";
            if (moveY > 0)
            {
                command = string.Format("F{0}", moveY);
                SendBluetoothData(command);
                _debugWindow.LogButtonPress(command);
            }

            if (moveY < 0)
            {
                command = string.Format("B{0}", -moveY);
                SendBluetoothData(command);
                _debugWindow.LogButtonPress(command);
            }
            
            if (positionX > 0)
            {
                command = string.Format("R{0}", positionX);
                SendBluetoothData(command);
                _debugWindow.LogButtonPress(command);
            }

            if (positionX < 0)
            {
                command = string.Format("L{0}", -positionX);
                SendBluetoothData(command);
                _debugWindow.LogButtonPress(command);
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
        int height = (int)(Mathf.Clamp(_heightSlider.value, 0, 9));
        _debugWindow.LogButtonPress("Height: " + height);
        SendBluetoothData("Z" + height);
    }
    
    public void SleepButtonHandler()
    {
        _debugWindow.LogButtonPress("Button Press: Sleep");

        SendBluetoothData("S0");

    }
    public void HomeButtonHandler()
    {
        _debugWindow.LogButtonPress("Button Press: Home");

        SendBluetoothData("H0");

    }

    public void ActionButtonHandler(string actionName)
    {
        _debugWindow.LogButtonPress("Action Button Press: " + actionName);

        SendBluetoothData("A" + actionName);

    }

    private void SendBluetoothData(string data)
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
