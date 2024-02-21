# Arduino-sketch-to-read-temperature-data-and-send-to-the-things-network
This sketch is for the Arduino MKR WAN 1300 (OR 1310)
The Arduino is connected to a temperature sensor DS18B20. 
The sketch reads the temperature at regular intervals, then connects to The Things Nework using LoRa (radio)
It requires a text file named 'arduino_secrets.h', which holds the identifier for your Arduino.
