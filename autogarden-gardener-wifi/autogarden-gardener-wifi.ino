/*
Cody Hasty - 663068521
CS362 Final Project - Gardener Unit - ESP8266 Code
Sends data from gardeners over serial to the Arduino, and sends response back
The circuit:

  ESP9266 Wifi Module:
 * Pin 1 - TX -> Arduino RX
 * Pin 2 - GND
 * Pin 3 - CH-PD -> 3.3V
 * Pin 4 - Unused
 * Pin 5 - RST - connect to button
 * Pin 6 - GPIO 0 - Unused
 * Pin 7 - VCC -> 3.3V
 * Pin 8 - RX -> Arduino TX

Heavily referenced the following tutorials:
https://www.arduino.cc/reference/en/libraries/wifi/
https://www.arduino.cc/reference/en/language/functions/communication/serial/

*/

// Wifi Library
#include <ESP8266WiFi.h>

// Wifi variables
const char* ssid = "CrowsNest"; //type your ssid
const char* password = "Suckstoyourassmar1138#"; //type your password

IPAddress hub(192,168,4,130);
WiFiClient gardener;

// Serial variables
String serialIn;

void setup() {
  // put your setup code here, to run once:

  // Serial
  Serial.begin(115200);
  delay(10);

  // Connect to Wifi
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  

}

void loop() {
  // put your main code here, to run repeatedly:

  // connected to network, now connect to hub
  gardener.connect(hub,80);
  delay(10);

  // If connected, read data from arduino and send to hub
  if (gardener.connected()) {

    // Signal to arduino that we are connected
    Serial.print("connected\n");
    delay(10);

    // wait for data from Arduino
    while(!Serial.available()) {
      delay(100);
    }
    // Read data and write to hub
    serialIn = Serial.readStringUntil('\r');
    gardener.println(serialIn);

    /*
    // Read temp and write to hub
    serialIn = Serial.readStringUntil('\n');
    gardener.print(serialIn + '\n');
    
    // Read moisture and write to hub
    serialIn = Serial.readStringUntil('\n');
    gardener.print(serialIn + '\n');
    */
    // flush buffer to keep data flow in sync
    Serial.flush();
  }
  else {
    Serial.print("No connection\n");
  }

  // delay for 3 seconds
  delay(3000);
  

}
