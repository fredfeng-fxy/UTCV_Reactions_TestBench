#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Keyboard.h"
int initialCValue=100000;
int threshold =500;
float startTime;
float currentTime;
float timeDiff;
int Started = 0;
int initValue=1;
/* Connect SCL to analog 5
Connect SDA to analog 4
Connect VDD to 3.3V DC
Connect GROUND to common ground */
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_199MS, TCS34725_GAIN_1X);
/* Initialise with specific int time and gain values */

// Car Motor
const int car_motor_speed = 5;
const int car_motor_fwd = 8; 
const int car_motor_rev = 9;

//Stirrer

const int stirrer_speed = 3;
const int stirrer_fwd = 6;
const int stirrer_rev = 7;


// button

const int switch_button = 10;
int reset_var = 1;

void setup(void) {
    Serial.begin(9600);

    if (tcs.begin()) {
      Serial.println("Found sensor");
    } else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1);
    }
// Now we're ready to get readings!

    // Car Motor Setup
    pinMode(car_motor_speed, OUTPUT);
    pinMode(car_motor_fwd, OUTPUT);
    pinMode(car_motor_rev, OUTPUT);

    // Stirrer Setup

    pinMode(stirrer_speed, OUTPUT);
    pinMode(stirrer_fwd, OUTPUT);
    pinMode(stirrer_rev, OUTPUT);
    
    // Car motor initialization
    digitalWrite(car_motor_fwd, LOW);
    digitalWrite(car_motor_rev, LOW);
    analogWrite(car_motor_speed, 255);

    // stirrer initialization
    digitalWrite(stirrer_fwd, LOW);
    digitalWrite(stirrer_rev, LOW);
    analogWrite(stirrer_speed, 255);

    // setting up switch
    pinMode(switch_button, INPUT);
}


void loop(void) {
uint16_t r, g, b, c, colorTemp, lux;
if(Serial.available()){
    char input = Serial.read();
    if (input == 115){
        startTime = millis();
        digitalWrite(car_motor_fwd, HIGH);
        digitalWrite(car_motor_rev, LOW);
        digitalWrite(stirrer_fwd, HIGH);
        digitalWrite(stirrer_rev, LOW);
        Started = 1;
    }
    if (input == 114){
        digitalWrite(car_motor_fwd, HIGH);
        digitalWrite(car_motor_rev, LOW);
        Started = 1;
    }
    if (input == 116){
        currentTime = millis();
        timeDiff = currentTime - startTime;
        Serial.print("Time (Interrupt): "); Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        // Car motor initialization
        digitalWrite(car_motor_fwd, LOW);
        digitalWrite(car_motor_rev, LOW);
        analogWrite(car_motor_speed, 255);
    
        // stirrer initialization
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);
        analogWrite(stirrer_speed, 255);
        Started = 0;
    }
}

if(digitalRead(switch_button) == HIGH){
  if(reset_var){
    startTime = millis();
    digitalWrite(car_motor_fwd, HIGH);  //car  stirrer forward
    digitalWrite(car_motor_rev, LOW);
    digitalWrite(stirrer_fwd, HIGH);
    digitalWrite(stirrer_rev, LOW);
    Started = 1;
    reset_var = 0;
   }
    
}

if(digitalRead(switch_button) == LOW){
    if(reset_var == 0){
      // Car motor initialization
      digitalWrite(car_motor_fwd, LOW);
      digitalWrite(car_motor_rev, LOW);
      // stirrer initialization
      digitalWrite(stirrer_fwd, LOW);
      digitalWrite(stirrer_rev, LOW);
      Started = 0;
      reset_var = 1;
    }
}

if(Started){
    tcs.getRawData(&r, &g, &b, &c);
    currentTime = millis();
    timeDiff = currentTime - startTime;
    if(timeDiff > 5000){
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);  
    }
    if(initValue){
      if(timeDiff > 6000){
        initialCValue=c;
        initValue=0;
        }
    }
    
    if(digitalRead(switch_button) == LOW){
      if(reset_var == 0){
        currentTime = millis();
        timeDiff = currentTime - startTime;
        Serial.print("Time (Interrupt): "); Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        // Car motor initialization
        digitalWrite(car_motor_fwd, LOW);
        digitalWrite(car_motor_rev, LOW);
    
        // stirrer initialization
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);
        Started = 0;
        reset_var = 1;
        
        }
    }
    
    if(c>(threshold+initialCValue)){
        Serial.print("Measured Time: ");
        Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        // Car motor initialization
        digitalWrite(car_motor_fwd, LOW);
        digitalWrite(car_motor_rev, LOW);
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);
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
