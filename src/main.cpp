#include <Arduino.h>
#include "motordriver.hpp"
#include "ultrasonic/ultrasonic.hpp"
#include <Servo.h>



// === PINS SERVO ===
#ifndef servoPin
#define servoPin 11
#endif


// === PINS DRIVER TB6612 ===
#ifndef AIN1
#define AIN1 7
#endif
#ifndef BIN1
#define BIN1 8
#endif
#ifndef PWMA
#define PWMA 5
#endif
#ifndef PWMB
#define PWMB 6
#endif
#ifndef STBY
#define STBY 3
#endif
// === PIN CAPTEUR ULTRASON ===

#ifndef trigPin
#define trigPin 13
#endif
#ifndef echoPin
#define echoPin 12
#endif



UltrasonicSensor sensor(trigPin, echoPin);
MotorDriver robot (AIN1, BIN1, PWMA, PWMB, STBY);







void setup() {

  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
}