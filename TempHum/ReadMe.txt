This folder contains the code for the Temperature and Humidity Sensor.
The code contains the following functions:
<Comment here to add new functions>

The sensor type is DHT11 Temperature-Humidity Sensor Module, which can be bought with this link: https://www.robotshop.com/ca/en/dht11-temperature-humidity-sensor-module.html

Tutorial for using this sensor can be found with this link: https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/

Sensor Specifications
Temperature
  Resolution : 1°C
  Accuracy : ±2℃
  Measuring range : 0°C ~ 50°C
Humidity
  Resolution : 1%RH
  Accuracy : ±5%RH (0~50°C)
  Measuring range : 20%RH ~ 90%RH (25°C)
Operating voltage : 3.3V ~ 5.5 V
Recommended storage condition
  Temperature : 10°C ~40°C
  Humidity : 60%RH or below

Code: 
#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(1200); // Has to > 1000ms
}

Code for CSV export(PLX-DAQ):
#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Computer Time,Temperature,Humidity");
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("DATA,TIME,");
  Serial.print(DHT.temperature);
  Serial.print(",");
  Serial.println(DHT.humidity);
  delay(1200);
}
