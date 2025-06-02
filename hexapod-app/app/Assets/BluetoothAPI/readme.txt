If you are upgrading from version 5.3.1 or older to version 5.4.0 please note the following changes:


All BluetoothHelper class events now have BluetoothHelper as their 1st parameter.

this is for better support of connection to multiple instance,
for example, previously you would do:

bluetoothHelper.OnConnected += () 
{
    //do something...
}


now you do :
bluetoothHelper.OnConnected += (helper) 
{
    //do something...
    //inside this function, you now have a reference to the caller of that function!
}

the above is applicable for all events.
another exampler:

bluetoothHelper.OnCharacteristicChanged += OnCharacteristicChanged;

void OnCharacteristicChanged (BluetoothHelper helper, byte[] value, BluetoothHelperCharacteristic characteristic)
{
    //do somthing
}

previously it was : 
void OnCharacteristicChanged (byte[] value, BluetoothHelperCharacteristic characteristic)
{
    //do somthing
}