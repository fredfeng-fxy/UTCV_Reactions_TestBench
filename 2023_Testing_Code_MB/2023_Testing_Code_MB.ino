#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "Keyboard.h" 
float threshold = 4;
float startTime;
float currentTime;
float timeDiff;
int Started = 0;
int ending = 0;
int initValue=1;

int read_idx = 0;
int MAX_IDX = 10;
int cur_sum = 0;
int cur_tf[11];
long cur_avg = 0;
int prev_tf[11];
int prev_sum = 0;
long prev_avg = 0;
long first_diff = 0;
float measured_time = 0;

/* Connect SCL to analog 5
Connect SDA to analog 4
Connect VDD to 3.3V DC
Connect GROUND to common ground */
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_120MS, TCS34725_GAIN_1X);
/* Initialise with specific int time and gain values */

// Car Motor 
const int car_motor_speed = 9;
//const int car_motor_fwd = 8; 
//const int car_motor_rev = 9;

//Stirrer

const int stirrer_speed = 9;
//const int stirrer_fwd = 10;
//const int stirrer_rev = 11;
const int stirrer_rot_speed = 80;
const int stirrer_slow_speed = 0;


// button

void setup(void) {
  
    Serial.begin(9600);

    if (tcs.begin()) {
      Serial.println("Found sensor");
    } else {
      Serial.println("No TCS34725 found ... check your connections");
      while (1);
    }
// Now we're ready to get readings!
    //setup queue
    for (int i = 0; i < (MAX_IDX + 1); i++){
      cur_tf[i] = 0;
      prev_tf[i] = 0;
    }
    // Car Motor Setup
    pinMode(car_motor_speed, OUTPUT);
    //pinMode(car_motor_fwd, OUTPUT);
    //pinMode(car_motor_rev, OUTPUT);

    // Stirrer Setup

    pinMode(stirrer_speed, OUTPUT);
    //pinMode(stirrer_fwd, OUTPUT);
    //pinMode(stirrer_rev, OUTPUT);
    
    // Car motor initialization
    //digitalWrite(car_motor_fwd, LOW);
    //digitalWrite(car_motor_rev, LOW);
    analogWrite(car_motor_speed, 0);

    // stirrer initialization
    //digitalWrite(stirrer_fwd, LOW);
    //digitalWrite(stirrer_rev, LOW);
    analogWrite(stirrer_speed, 0);

}


void loop(void) {
uint16_t r, g, b, c, colorTemp, lux;
if(Serial.available()){
    char input = Serial.read();
  // Type letters into the serial monitor to access various options
    if (input == 115){
        // s: Start the car, including the stirrer and the main motor
        startTime = millis();
        //digitalWrite(car_motor_fwd, HIGH);
        //digitalWrite(car_motor_rev, LOW);
        //digitalWrite(stirrer_fwd, HIGH);
        //digitalWrite(stirrer_rev, LOW);
        analogWrite(stirrer_speed, stirrer_rot_speed);
        Started = 1;
        initValue = 1;
    }
    if (input == 114){
        // r: Restart the car motor, keeping the current timer
        //digitalWrite(car_motor_fwd, HIGH);
        //digitalWrite(car_motor_rev, LOW);
        Started = 1;
    }
    if (input == 116){
        // t: Stops the car and measurement, including timers
        currentTime = millis();
        timeDiff = currentTime - startTime;
        Serial.print("Time (Interrupt): "); Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        // Car motor initialization
        //digitalWrite(car_motor_fwd, LOW);
        //digitalWrite(car_motor_rev, LOW);
        analogWrite(car_motor_speed, 255);
    
        // stirrer initialization
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);
        analogWrite(stirrer_speed, 0);
        Started = 0;
    }
}


if(Started){
    tcs.getRawData(&r, &g, &b, &c);
    currentTime = millis();
    timeDiff = currentTime - startTime;
    if(timeDiff > 5000){
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);  
        analogWrite(stirrer_speed, stirrer_slow_speed);
    }

    cur_tf[read_idx] = c;
    cur_sum = 0;
    for (int i = 0; i < (MAX_IDX + 1); i++){
      cur_sum += cur_tf[i];
    }
    
    cur_avg = cur_sum / (MAX_IDX + 1);

    prev_sum = 0;
    for (int i = 0; i < (MAX_IDX + 1); i++){
      prev_sum += prev_tf[i];
    }

    prev_avg = prev_sum / (MAX_IDX + 1);
    
    first_diff = cur_avg - prev_avg;
    if(initValue){
      if(timeDiff > 15000){
        initValue=0;
        }
    }else{
      if((ending <1) & (first_diff > threshold)){
        // Car motor initialization
        Serial.print("Measured Time: ");
        Serial.print(timeDiff/1000, DEC);
        Serial.println(" ");
        // // Car motor initialization
        // digitalWrite(car_motor_fwd, LOW);
        // digitalWrite(car_motor_rev, LOW);
         //digitalWrite(stirrer_fwd, LOW);
         //digitalWrite(stirrer_rev, LOW);
        measured_time = timeDiff;
        // Started = 0;
        ending = 20;
      }else if(ending == 1){
        Started = 0;
        measured_time = 0;
        ending = 0;
        // Car motor initialization
        //digitalWrite(car_motor_fwd, LOW);
        //digitalWrite(car_motor_rev, LOW);
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);
      }else{
        ending = ending -1;
      }
    }
    prev_tf[read_idx] = c;
    read_idx++;
    if (read_idx > MAX_IDX){
      read_idx = 0;
    }
    
    
//    if(c>(threshold+initialCValue)){
//        Serial.print("Measured Time: ");
//        Serial.print(timeDiff/1000, DEC);
//        Serial.println(" ");
//        // Car motor initialization
//        digitalWrite(car_motor_fwd, LOW);
//        digitalWrite(car_motor_rev, LOW);
//        digitalWrite(stirrer_fwd, LOW);
//        digitalWrite(stirrer_rev, LOW);
//        Started = 0;
//    }
    Serial.print("R:"); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G:"); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B:"); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C:"); Serial.print(c, DEC); Serial.print(" ");
    Serial.print("cur_avg"); Serial.print(cur_avg, DEC); Serial.print(" ");
    Serial.print("prev_avg"); Serial.print(prev_avg, DEC); Serial.print(" ");
    Serial.print("read_idx"); Serial.print(read_idx, DEC); Serial.print(" ");
    Serial.print("Time:"); Serial.print(timeDiff/1000, DEC); Serial.print(" ");
    Serial.print("Measured Time: "); Serial.print(measured_time/1000, DEC); Serial.print(" ");
    Serial.println(" ");
  }
}
