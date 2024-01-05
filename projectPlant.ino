/* Fill-in information from Blynk Device Info tab  here */
#define BLYNK_TEMPLATE_ID "TMPL4xxN3Gd6O"
#define BLYNK_TEMPLATE_NAME "ProjectPlant"
#define BLYNK_AUTH_TOKEN "1igobC5jsTj7haQeBUS99mI-tLOY35FW"

#include <SPI.h>
#include <WiFi.h>
#include <BlynkSimpleWifi.h>
#include <Arduino_MKRIoTCarrier.h>

//MKRIoTCarrier carrier; //an object of type MKRIoTCarrier is created
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "VODAFONE-30C0";
char pass[] = "YmXHN7J3RRsMFFbC";

BlynkTimer timer;
MKRIoTCarrier carrier;

// This function is called every time the Virtual Pin 0 state changes
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
  // Don't send more that 10 values per second.
  float temperature = carrier.Env.readTemperature();
  Blynk.virtualWrite(V1, temperature);
}

const int sensorPin = A0; // Analog input pin that the sensor is attached to

int sensorValue = 0; // Value read from the sensor
int threshold = 600; // Threshold value to determine if the soil is wet or dry
int threshold2 = 650; // Threshold value to determine if the soil is dry or very dry

void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(2000L, writeTemperature);
  carrier.withCase();                         //Remove this if your arduino is NOT in it's case/housing
  carrier.Buttons.updateConfig(100, TOUCH0);  //Remove this if your arduino is NOT in it's case/housing
  carrier.begin();
  carrier.display.fillScreen(0x07E0);
}


void loop() {
  sensorValue = analogRead(sensorPin); // Read the sensor value
  Serial.println(sensorValue); // Print the sensor value to the serial monitor

  if (sensorValue > threshold && sensorValue < threshold2) { // If the soil is dry: between 600 and 650
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

  if (sensorValue > threshold2) { // If the soil is very dry - a reading over 650
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
    int red = 0, green = 0, blue = 200; //when the reading is less than the threshold i.e. not dry
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




