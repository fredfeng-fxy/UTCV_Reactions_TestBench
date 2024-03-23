//libraries
#include <ezButton.h>
#include <Wire.h>
#include <Keyboard.h>

//customize
const int threshold = XX; // Change this value to what your color sensor reads to

//stirrer speeds (can change if stirrer too fast)
const int stirrer_rot_speed = 255;

//init variables
float startTime;
float currentTime;
float timeDiff;
int Started = 0;
int stopped = 0;
int notDetecting = 1; // start by not detecting with color sensor

// Color sensor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int clearFrequency = 0;

/// Connection pins to other parts of the car!
const int BUTTON_PIN = 9; // the pin number of the pushbutton pin
const int stirrer_motor = 10; // the pin number of the stirrer motor's pwm control
const int car_motor = 11; // the pin number of the car motor's pwm control

ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin BUTTON_PIN;
void(* resetFunc) (void) = 0; //declare reset function @ address 0

//colour sensing loop
void main_loop() {

    currentTime = millis();
    timeDiff = currentTime - startTime;
    // stop stirring after 30 seconds
    if(timeDiff > 30000){
        analogWrite(stirrer_motor, 0);
    }
    
    // Setting RED (R) filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    
    // Reading the output frequency
    redFrequency = pulseIn(sensorOut, LOW);
    
    // Setting GREEN (G) filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    
    // Reading the output frequency
    greenFrequency = pulseIn(sensorOut, LOW);
  
    // Setting BLUE (B) filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    
    // Reading the output frequency
    blueFrequency = pulseIn(sensorOut, LOW);

    // Setting Clear (C) filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,LOW);
    
    // Reading the output frequency
    clearFrequency = pulseIn(sensorOut, LOW);


    Serial.print("R: "); Serial.print(redFrequency, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(greenFrequency, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(blueFrequency, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(clearFrequency, DEC); Serial.print(" ");
    Serial.print("Time: "); Serial.print(timeDiff/1000, DEC); Serial.print(" ");
    Serial.println(" ");
    
    if(notDetecting){
      // start detecting with color sensor after 10 seconds to account for stirring instability for the first few seconds
      if(timeDiff > 10000){
        notDetecting=0;
        }
    }else{

      // You can create a more accurate version of this!
      stopped = clearFrequency > threshold;
      // Think of ways to process this threshold based on your data
      
      if(stopped){ // if above threshold, end
        stop_time = timeDiff;
        // Car motor stop
        analogWrite(car_motor,0);
        // stop stirrer
        analogWrite(stirrer_motor,0);
        Started = 0;
        resetFunc();
        while(true);
        
      }
    }
}


//setup
void setup() {
  // Car Motor Setup
  pinMode(car_motor, OUTPUT);

  // Stirrer Setup
  pinMode(stirrer_motor, OUTPUT);
  
  // Car motor initialization
  analogWrite(car_motor, 0);

  // stirrer initialization
  analogWrite(stirrer_motor, 0);

  // Color Sensor Code
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);         // initialize serial (only needed for if connected to computer

  //setup button
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  //wait a second
  delay(1000);

}


void loop() {
  
  button.loop(); // MUST call the button.loop() function before checking pressed

  //loop till button is pressed, then wait until button released to start
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
    // control LED arccoding to the toggled sate
  }

  //turn off arduino led to indicate started
  if(Started==1){
    startTime = millis();

    //start stirrer and car
    analogWrite(stirrer_motor, stirrer_rot_speed);
    analogWrite(car_motor, 255);

    //detect colour change
    while(true){
      main_loop();
      delay(1);
    }
  }
}
