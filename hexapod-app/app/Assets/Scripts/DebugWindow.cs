using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class DebugWindow : MonoBehaviour
{
    public Text OutputText;
    private LinkedList<string> _debugLines = new LinkedList<string>();
    const int maxLines = 5;

    // Start is called before the first frame update
    void Start()
    {
        writeDebugText("Starting...");
    }

    public void LogButtonPress(string buttonName)
    {
        writeDebugText(buttonName);
    }

    public void LogBluetooth(string bluetoothMessage)
    {
        writeDebugText("Bluetooth - " + bluetoothMessage);
    }

    private void writeDebugText(string debugMessage)
    {
        if (_debugLines.Count == maxLines)
        {
            _debugLines.RemoveLast();
        }
        _debugLines.AddFirst(System.DateTime.Now.ToString() + ": " + debugMessage);

        string result = "";
        foreach (var nextLine in _debugLines)
        {
            result = result + nextLine + System.Environment.NewLine;
        }

        OutputText.text = result;
    }
}
