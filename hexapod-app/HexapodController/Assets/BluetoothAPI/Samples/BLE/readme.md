# BLE

This sample provides a script for running the example on raspberry pi
(This is also valid for Arduino having BLE)
2 scripts are to be placed on raspberry pi

install required tools:

```bash
sudo apt install nodejs
sudo npm install -g n
sudo n 8.9.0
sudo apt-get install bluetooth bluez libbluetooth-dev libudev-dev
sudo npm install bleno 
```

To start it, run the following

```bash
nodejs ~/node-bluetooth-hci-socket/main.js
```
