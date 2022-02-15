#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Keyboard.h"

int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int threshold = 42;
float startTime;
float currentTime;
float timeDiff;
int Started = 0;
 
/* Connect SCL to analog 5
Connect SDA to analog 4
Connect VDD to 3.3V DC
Connect GROUND to common ground */
 
/* Initialise with default values (int time = 2.4ms, gain = 1x) */

Adafruit_TCS34725 tcs = Adafruit_TCS34725();
 
/* Initialise with specific int time and gain values */

 
void setup(void) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    Serial.begin(9600);

    if (tcs.begin()) {
    Serial.println("Found sensor");
    } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
}
 
// Now we're ready to get readings!
}
 
void loop(void) {
uint16_t r, g, b, c, colorTemp, lux;

if(Serial.available()){
    char input = Serial.read();

    if (input == 115){
        startTime = millis();
        Started = 1;
    }

    if (input == 114){
        Started = 1;
    }

    if (input == 116){
        currentTime = millis();
        timeDiff = currentTime - startTime;
        Serial.print("Time (Interrupt): "); Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        Started = 0;
    }
}


if(Started){
    tcs.getRawData(&r, &g, &b, &c);
    currentTime = millis();
    timeDiff = currentTime - startTime;
 
    if(c>threshold){
        Serial.print("Measured Time: ");
        Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        Started = 0;
    }
    Serial.print("R:"); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G:"); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B:"); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C:"); Serial.print(c, DEC); Serial.print(" ");
    Serial.print("Time: "); Serial.print(timeDiff/1000, DEC);
    Serial.println(" ");
}

}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
