//Import the required libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Declear two variables called "temperature", "temperature_new", "difference"
float temperature,temperature_new, difference;

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
  
  sensors.requestTemperatures();  // Get the initial temperature when the system starts up
  temperature = sensors.getTempCByIndex(0); // Store the initial temperature as "Temperature"
  
  //print the initial temperature in Celsius
  Serial.println("Sensor ready");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" ºC");
}

  
void loop(void)
{ 
  // Get new temperature
  sensors.requestTemperatures(); 
  temperature_new = sensors.getTempCByIndex(0);
  
  difference = abs(temperature_new - temperature); // Claculate temperature change between two consecutive meassurements

  //If the change is greater than 0.2, print out the new temperature, otherwise do nothing
  if(difference > 0.1){
    //print the temperature in Celsius
    Serial.print("Temperature: ");
    Serial.print(temperature_new);
    Serial.println(" ºC");
  }
  else{
    // Do nothing
  }

  //Update the store temperature value
  temperature = temperature_new; 
  delay(200);
}
