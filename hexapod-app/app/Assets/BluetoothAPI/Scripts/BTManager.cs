using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using ArduinoBluetoothAPI;
using System;

public class BTManager : MonoBehaviour {

	// Use this for initialization

	string message;
	private BluetoothHelper BTHelper;

	public GameObject[] spheres;

	public Material[] materials;

	private string x;

	void Start () {

		try{
			x="";
			BTHelper = BluetoothHelper.GetInstance("HC-05");
			BTHelper.setLengthBasedStream();
			BTHelper.OnConnected += OnBluetoothConnected; //OnBluetoothConnected is a function defined later on
			
			BTHelper.OnDataReceived += (helper) => { 
				try{
				string xx = helper.Read();
				char[] data = xx.ToCharArray();

				if(data.Length != 3)
				{
					return;
				}

				int i = 0;
				if(data[0] != 'S')
					return;
				if(data[1] == 'E')
					i=1;
				if(data[2] > 7)
					return;

				spheres[data[2]-2].GetComponent<Renderer>().material = materials[i];
				}catch(Exception ex)
				{
					x += ex.Message;
				}
			};
			
		}catch(Exception ex){
			Debug.Log(ex);
			x = ex.ToString();
		}
	}

	void OnBluetoothConnected(BluetoothHelper helper)
	{
		try{
			helper.StartListening();
			StartCoroutine(blinkLED());
			
		}catch (Exception ex){
			x += ex.ToString();
			Debug.Log(ex.Message);
		}
		
	}

	IEnumerator blinkLED()
	{
		byte[] turn_on = new byte[]{(byte)'E' /*E stands for enable */, 2};
		byte[] turn_off = new byte[]{(byte)'D' /*D stands for disable */, 2};
		x += BTHelper.isConnected().ToString();
		
		while(BTHelper.isConnected())
		{
			Debug.Log("ON");
			for(byte i=2; i<8; i++)
			{
				turn_on[1] = i;
				try{
					BTHelper.SendData(turn_on);
				}catch(Exception){}
				yield return new WaitForSeconds(0.3f);
			}
			Debug.Log("OFF");
			for(byte i=2; i<8; i++)
			{
				turn_off[1] = i;
				try{
					BTHelper.SendData(turn_off);
				}catch(Exception){}
				yield return new WaitForSeconds(0.3f);
			}
		}

	}
	
	void OnGUI()
	{

		if(BTHelper == null)
			return;
		

		BTHelper.DrawGUI();

		if(!BTHelper.isConnected())
		if(GUI.Button(new Rect(Screen.width/2-Screen.width/10, Screen.height/10, Screen.width/5, Screen.height/10), "Connect"))
		{
			if(BTHelper.isDevicePaired())
				BTHelper.Connect (); // tries to connect
		}

		if(BTHelper.isConnected())
		if(GUI.Button(new Rect(Screen.width/2-Screen.width/10, Screen.height - 2*Screen.height/10, Screen.width/5, Screen.height/10), "Disconnect"))
		{
			BTHelper.Disconnect ();
		}
	}

	void OnDestroy()
	{
		if(BTHelper!=null)
			BTHelper.Disconnect();
	}
}
