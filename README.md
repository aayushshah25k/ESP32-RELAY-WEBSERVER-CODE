IS IS WORKING CODE FOR ESP32 WEBSERVER WHICH IS CONTROLLED WIRELESSLY VIA PHONE.

Install Arduino IDE and add ESP32 board from Boards Manager.
Connect ESP32 to laptop using USB cable and select correct COM port.
Open a new sketch and paste the ESP32 web server code.
In the code, enter your hotspot details:
ssid = your mobile hotspot name
password = your hotspot password
Turn ON mobile hotspot and set it to 2.4 GHz band.
Click Upload in Arduino IDE and wait until upload is complete.
Open Serial Monitor and set baud rate to 115200.
Note the IP address shown (example: 192.168.x.x).
Connect your phone to the same hotspot.
Open browser in phone, enter the IP address, and control the relays (Light/Fan).
