//libraries
#include <Servo.h>
#include <ezButton.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <Keyboard.h>

//customize
const float threshold = 4;
const int min_time = 20000;

const int max_angle = 145;
const int min_angle = 75;
const int servo_delay = 40;

//stirrer speeds
const int stirrer_rot_speed = 255;
const int stirrer_slow_speed = 0;


//init variables
float startTime;
float currentTime;
float timeDiff;
int initValue=1;
int Started = 0;

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

/// pins
const int BUTTON_PIN = 4; // the pin number of the pushbutton pin
const int LED_PIN    = 13; // the pin number of the LED pin (always 13)
const int SENSOR_LED = 7; // the pin number of colour sensor's on-board led
const int SERVO_PIN = 9; // the pin number of the servo motor's pwm control
const int car_motor = 11; // the pin number of the car motor's pwm control
const int stirrer_motor = 10; // the pin number of the stirrer motor's pwm control


ezButton button(BUTTON_PIN);  // create ezButton object that attach to pin BUTTON_PIN;
Servo myservo; //servo motor

//colour sensing loop
void main_loop() {
uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    currentTime = millis();
    timeDiff = currentTime - startTime;
    if(timeDiff > 5000){
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);  
        analogWrite(stirrer_motor, stirrer_slow_speed);
    }
    if(timeDiff > 30000){
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);
        analogWrite(stirrer_motor, 0);
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
    
    Serial.print("R:"); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G:"); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B:"); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C:"); Serial.print(c, DEC); Serial.print(" ");
    Serial.print("cur_avg"); Serial.print(cur_avg, DEC); Serial.print(" ");
    Serial.print("prev_avg"); Serial.print(prev_avg, DEC); Serial.print(" ");
    Serial.print("read_idx"); Serial.print(read_idx, DEC); Serial.print(" ");
    Serial.println(" ");
    
    if(initValue){
      if(timeDiff > min_time){
        initValue=0;
        }
    }else{
      if(first_diff > threshold){ //if above threshold, end
        // Car motor stop
        analogWrite(car_motor,0);
        analogWrite(stirrer_motor,0);
        digitalWrite(SENSOR_LED,LOW);
        //digitalWrite(car_motor_fwd, LOW);
        //digitalWrite(car_motor_rev, LOW);
        //digitalWrite(stirrer_fwd, LOW);
        //digitalWrite(stirrer_rev, LOW);
        Started = 0;
        myservo.write(160);
        while(true);
      }
    }
    prev_tf[read_idx] = c;
    read_idx++;
    if (read_idx > MAX_IDX){
      read_idx = 0;
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

  //attach servo
  myservo.attach(SERVO_PIN);
  myservo.write(160);
  
  Serial.begin(9600);         // initialize serial (only needed for if connected to computer

  // setup LEDs
  pinMode(LED_PIN, OUTPUT);   // set arduino led pin to output mode
  digitalWrite(LED_PIN,LOW);
  pinMode(SENSOR_LED, OUTPUT);
  digitalWrite(SENSOR_LED,LOW);

  //setup button
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  //wait a second
  delay(1000);

  //connect to colour sensor
  int connected = 0;
  while (!connected){
      if (tcs.begin()) {
      Serial.println("Found sensor");
      connected = 1;
      digitalWrite(LED_PIN,HIGH); //turn on these lights to indicate sensor detected
      digitalWrite(SENSOR_LED, HIGH);
    } else {
      Serial.println("No TCS34725 found ... check your connections");
      delay(100);
    }
  }

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
    digitalWrite(LED_PIN, LOW);

    //auto inject
    for (int i = max_angle;i>min_angle+1; i = i-2){
      myservo.write(i);
      delay(servo_delay);
    }

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
