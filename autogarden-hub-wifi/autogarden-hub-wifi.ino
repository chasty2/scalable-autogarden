/*
Cody Hasty - 663068521
CS362 Final Project - Central Hub - ESP8266 Code
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

#include <ESP8266WiFi.h>
 
const char* ssid = "CrowsNest";//type your ssid
const char* password = "Suckstoyourassmar1138#";//type your password
 
WiFiServer server(80);//Service Port


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

  // connected, start server and signal to Arduino
  server.begin(80);
  Serial.println("connected");

  // Send ssid and IP to serial when wifi is connected
  Serial.println(ssid);
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  // Wait for client to connect
  WiFiClient gardener = server.available();
  if (!gardener) {
    return;
  }

  // Wait for connected client to send data
  while(!gardener.available()) {
    delay(2);
  }

  // Signal to arduino that data is being sent
  Serial.print("data\n");

  /* Collect Data
   *  
   * Format: 
   * "Plant_number\n"
   * "Temp\n"
   * "Moisture\n"
   *  
   */
  
  // Read Plant Number and send to Arduino
  String request = gardener.readStringUntil('\n');
  Serial.print(request + '\n');
  delay(3);
  
  // Read Temperature and send to Arduino
  request = gardener.readStringUntil('\n');
  Serial.print(request + '\n');
  delay(3);

  // Read Moisture and send to Arduino
  request = gardener.readStringUntil('\n');
  Serial.print(request + '\n');
  delay(3);
  
  // delay for 3 seconds
  delay(1000);
  
}
