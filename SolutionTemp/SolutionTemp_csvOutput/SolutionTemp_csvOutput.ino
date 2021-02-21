//Import the required libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Declear two variables called "temperature"
float temperature;

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup(void)
{
  sensors.begin();  // Start up the library, the default resolution of the senser is 12-bits, corresponding to a increment of 0.0625ºC
  // sensors.setResolution(12); // If want to change the resolution, uncommnet this line of code; the resolution can be 9, 10, 11, or 12, corresponding to increments of 0.5°C, 0.25°C, 0.125°C, and 0.0625°C, respectively
  Serial.begin(9600);
  //print start information
  Serial.println("Sensor ready");
}

  
void loop(void)
{ 
  // Get the temperature
  sensors.requestTemperatures(); 
  temperature = sensors.getTempCByIndex(0);
  // Print Temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" ºC");

  delay(200);
}
