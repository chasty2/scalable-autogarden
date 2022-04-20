/*
Cody Hasty - 663068521
CS362 Final Project - Gardener Unit - Arduino Code
Sends data from sensors over serial to the Arduino, and acts on response
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
https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
https://www.tutorialspoint.com/arduino/arduino_temperature_sensor.htm
https://learn.sparkfun.com/tutorials/soil-moisture-sensor-hookup-guide

*/
// String to read data from serial
String serialIn;

// Moisture variables
int moistureReading = 0;
int moisture = 0; 
int soilPin = A0;
int soilPowerPin = 8;

// Temperature Values
int tempPin = A1;
float tempReading = 0;
int tempCelsius = 0;

void setup() {
  // put your setup code here, to run once:

  // built-in LED used for debugging
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup moisture pin
  pinMode(soilPowerPin, OUTPUT);
  digitalWrite(soilPowerPin, LOW);

  // Serial
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Collect sensor readings
  tempCelsius = readTemp();
  moisture = readSoil();

  // Read from Serial
  if(Serial.available() > 0) {
    
    serialIn = Serial.readStringUntil('\n');
    delay(10);
    
    // Handle next request based on first string read
    if(serialIn.indexOf("connected") != -1) {
      sendData();
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(serialIn.indexOf("No connection") != 0) {
      digitalWrite(LED_BUILTIN, LOW);
    }

    // Flush buffer
    Serial.flush();
  }

  // delay for 4 seconds
  delay(4000);
}

// function to read moisture sensor
int readSoil()
{

  //turn D8 "On" for 10 ms
  digitalWrite(soilPowerPin, HIGH); 
  delay(10);

  //Read the SIG value form sensor 
  moistureReading = analogRead(soilPin);

  // turn D8 off
  digitalWrite(soilPowerPin, LOW);

  // map reading to 0-100 and return
  moisture = map(moistureReading,0,1023,0,100);
  return moisture;
}

// function to read temperature sensor
float readTemp() {
  
  tempReading = analogRead(tempPin);
  // convert the analog volt to its temperature (Celsius) equivalent
  tempCelsius = tempReading * 0.48828125;
  return tempCelsius;
}

/* send data to hub if connected
   *  
   * Format: 
   * "Plant_number\n"
   * "Temp\n"
   * "Moisture\n"
   */
void sendData() {
  Serial.print("Aloe Vera\n");

  //Serial.println(tempCelsius);
  Serial.print(String(tempCelsius) + '\n');

  //Serial.println(moisture);
  Serial.print(String(moisture) + '\n');
}
