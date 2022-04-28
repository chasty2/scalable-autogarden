/*
Cody Hasty - 663068521
CS362 Final Project - Central Hub - Arduino Code
Receives data from gardeners via wifi -> serial, and outputs to an LCD screen
The circuit:

  LCD:
 * LCD RS pin to digital pin 11
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

  ESP9266 Wifi Module:
 * Uses TX and RX to communicate with Arduino

Heavily referenced the following tutorials:
https://docs.arduino.cc/learn/electronics/lcd-displays
https://www.arduino.cc/reference/en/language/functions/communication/serial/

*/

// LCD library
#include <LiquidCrystal.h>

// Declare LCD variables
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Strings for Serial reading
String serialIn;
String ssid;
String ip;
String temp;
String moisture;
String plantName;


void setup() {
  // put your setup code here, to run once:
  
  // LCD
  lcd.begin(16,2);

  // Serial
  Serial.begin(115200);

  // Wait for Wifi connection
  lcd.setCursor(0,0);
  lcd.print("Connecting...");
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Read from Serial
  if(Serial.available() > 0) {
    
    serialIn = Serial.readStringUntil('\n');
    delay(10);
    
    // Handle next request based on first string read
    if(serialIn.indexOf("connected") != -1) {
      readWifiInfo();
      printWifiInfo();
    } 
    if(serialIn.indexOf("data") != -1) {
      readData();
    }
  } else {
    // no data
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No data");
    lcd.setCursor(0,1);
    lcd.print(ip);
  }

  Serial.flush();
  delay(2000);

}

// This function prints the wifi info of the router we are connected to
void readWifiInfo() {
  
  // Read ssid from serial
  ssid = Serial.readStringUntil('\r');
  // remove newline
  Serial.readStringUntil('\n');
  delay(10);

  // Read ip address
  ip = Serial.readStringUntil('\r');
  // remove newline
  Serial.readStringUntil('\n');
  delay(10);
}

// Print Wifi info to the LCD
void printWifiInfo() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print(ip);

  delay(5000);
}

// Reads data from the wifi module, and outputs to LCD
void readData() {

  // Wait for data
  while(!Serial.available()) {
    delay(100);
  }

  /* 
   * Data Format: 
   * "Plant_number\nTemp\nMoisture\n\r\n"
   */ 
  // Read plant number
  plantName = Serial.readStringUntil('\n');
  delay(2);

  // Read Temp
  temp = Serial.readStringUntil('\n');
  delay(2);

  // Read Moisture
  moisture = Serial.readStringUntil('\r');
  delay(2);

  // flush buffer
  Serial.flush();

  // Output to LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(plantName);
  lcd.setCursor(0,1);
  lcd.print(temp + "C");
  lcd.setCursor(5,1);
  lcd.print(moisture + "%");

  
}
