#include <MKRWAN.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LoRaModem modem;

// Please enter your sensitive data in the arduino_secrets.h tab
#include "arduino_secrets.h"

// Data wire is conntect to the Arduino digital pin 4
#define ONE_WIRE_BUS 4 // labelled ~4 on the mkr wan board

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float tempVariable = 0; // initialises a variable to take temperature data

// We need the details below to connect your device to LoRa
// Read arduino secrets
String appEui = APP_EUI;
String appKey = APP_KEY;

void setup() {
  // Put your setup code here, to run once:
    
  Serial.begin(9600);
  while (!Serial && millis() < 10000); //This line is needed to work with batteries https://forum.arduino.cc/t/it-does-not-work-powered-by-2xaa-batteries/520097/5

  sensors.begin();

  // The code below connects your device to LoRa
  if (!modem.begin(EU868)) { // change this to your regional band (eg. US915, AS923, ...)
    Serial.println("Failed to start module");
    while (1) {}
  };

  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  int connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {

    Serial.println("Something went wrong; are you indoors? Move near a window and retry");
    while (1) {}

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  // Populate our tempVariable variable.
  tempVariable = (sensors.getTempCByIndex(0));

  byte payload[1]; // The [1] means we are sending one item in the array. Arrays start at zero.
  payload[0] = tempVariable;

  delay(1000); // Gives it time to execute.
  
  modem.beginPacket();
  modem.write(payload,1);
  int err = modem.endPacket(false);
  if (err > 0) {
    Serial.println("Data Sent");
  } else {
    Serial.println("Error");
  }
  Serial.print("temp data sent is: "); 
  Serial.println(tempVariable);
  
  delay(30000);

}
