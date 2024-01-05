/*plant monitoring system
#define BLYNK_TEMPLATE_ID "TMPL4xxN3Gd6O"
#define BLYNK_TEMPLATE_NAME "ProjectPlant"
#define BLYNK_AUTH_TOKEN "1igobC5jsTj7haQeBUS99mI-tLOY35FW"

#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>

//MKRIoTCarrier carrier; //an object of type MKRIoTCarrier is created
// WiFi credentials.
char ssid[] = "***************";
char pass[] = "********************";

BlynkTimer timer;
MKRIoTCarrier carrier;

BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int lightsOn = param.asInt();
  if (lightsOn) {
    carrier.display.fillScreen(0xF800);
  } else {
    carrier.display.fillScreen(0x07E0);
  }
}

// This function sends temperature every second to Virtual Pin 1.
void writeTemperature() {
  float temperature = carrier.Env.readTemperature();
  Blynk.virtualWrite(V1, temperature);
}

const int sensorPin = A0; // Analog input pin that the sensor is attached to

int sensorValue = 0; // Value read from the sensor
int threshold = 700; // Threshold value to determine if the soil is wet or dry
int threshold2 = 750; // Threshold value to determine if the soil is dry or very dry

void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(2000L, writeTemperature);
  carrier.withCase();                         
  carrier.Buttons.updateConfig(100, TOUCH0);  
  carrier.begin();
  carrier.display.fillScreen(0x07E0);
}


void loop() {
  sensorValue = analogRead(sensorPin); // Read the sensor value
  Serial.println(sensorValue); // Print the sensor value to the serial monitor

  if (sensorValue > threshold && sensorValue < threshold2) { // If the soil is dry: between 700 and 750
    int red = 255, green = 255, blue = 0;
    int ledColour = carrier.leds.Color(red, green, blue); //create colour to display on LEDs
    carrier.leds.fill(ledColour); 	//Set all LEDs to yellow
    carrier.leds.show(); 
  
    Serial.print("Soil dry");
    Blynk.run();
    timer.run();

    delay(2000); // Wait for 2 seconds
    carrier.leds.fill(0); //Set all LEDs to no colour(off)
    carrier.leds.show(); 	
    delay(2000); // Wait for 2 seconds
  }

  if (sensorValue > threshold2) { // If the soil is very dry - a reading over 750
    int red = 255, green = 0, blue = 0;
    int ledColour = carrier.leds.Color(red, green, blue); //create colour to display on LEDs
    carrier.leds.fill(ledColour); //set leds to red
    carrier.leds.show(); 
    
    Serial.println("Soil very dry");
    Blynk.run();
    timer.run();

    delay(1000); // Wait for a second
    carrier.leds.fill(0);//Set all LEDs to no colour(off)
    carrier.leds.show(); 	
    delay(1000); // Wait for a second

  }else
    {
    int red = 138, green = 43, blue = 226; //when the reading is less than the threshold i.e. not dry
    int ledColour = carrier.leds.Color(red, green, blue); //create colour to display on LEDs
    carrier.leds.fill(ledColour); //set leds to blue
    carrier.leds.show(); 
    Serial.println("Soil ok");

    Blynk.run();
    timer.run();

    delay(2000); // Wait for 2 seconds
    carrier.leds.fill(0); 			//Set all LEDs to no colour(off)
    carrier.leds.show(); 	
    delay(2000); // Wait for 2 seconds

    }
  }




