/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-button-toggle-led
 */

#include <ezButton.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <Keyboard.h>
float threshold =5;
float startTime;
float currentTime;
float timeDiff;
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

/* Connect SCL to analog 5
Connect SDA to analog 4
Connect VDD to 3.3V DC
Connect GROUND to common ground */
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_120MS, TCS34725_GAIN_1X);
/* Initialise with specific int time and gain values */

/// constants won't change
const int BUTTON_PIN = 12; // the number of the pushbutton pin
const int LED_PIN    = 13; // the number of the LED pin

int Started = 0;

// Car Motor
const int car_motor_speed = 6;
const int car_motor_fwd = 7; 
const int car_motor_rev = 8;

//stirrer
const int stirrer_speed = 9;
const int stirrer_fwd = 10;
const int stirrer_rev = 11;

ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin 7;

// variables will change:
int ledState = LOW;   // the current state of LED

void main_loop() {
uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    currentTime = millis();
    timeDiff = currentTime - startTime;
    if(timeDiff > 5000){
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);  
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
      if(timeDiff > 10000){
        initValue=0;
        }
    }else{
      if(first_diff > threshold){
        // Car motor initialization
        digitalWrite(car_motor_fwd, LOW);
        digitalWrite(car_motor_rev, LOW);
        digitalWrite(stirrer_fwd, LOW);
        digitalWrite(stirrer_rev, LOW);
        Started = 0;
      }
    }
    prev_tf[read_idx] = c;
    read_idx++;
    if (read_idx > MAX_IDX){
      read_idx = 0;
    }
}

void setup() {
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
    analogWrite(stirrer_speed, 90);
  
  Serial.begin(9600);         // initialize serial
  pinMode(LED_PIN, OUTPUT);   // set arduino pin to output mode
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button.loop(); // MUST call the loop() function first
  if(button.isPressed()) {
    Serial.println("The button is pressed");

    while(Started==0){
      button.loop();
      if(button.isReleased()){
        Started = 1;
      }
      delay(1);
    }
    // toggle state of LED
    // control LED arccoding to the toggleed sate
  }
  if(Started==1){
    startTime = millis();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    digitalWrite(stirrer_fwd, HIGH);
    digitalWrite(car_motor_fwd, HIGH);

    while(true){
      main_loop();
      delay(1);
    }
  }
}
