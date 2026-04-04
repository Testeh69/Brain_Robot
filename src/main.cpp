#include <Arduino.h>
#include "motordriver.hpp"
#include "ultrasonic.hpp"
#include <Servo.h>



// === PINS SERVO ===
#define servoPin 11


// === PINS DRIVER TB6612 ===
#define AIN1 7
#define BIN1 8
#define PWMA 5
#define PWMB 6
#define STBY 3

// === PIN CAPTEUR ULTRASON ===
#define trigPin 13
#define echoPin 12



UltrasonicSensor sensor(trigPin, echoPin);
MotorDriver robot (AIN1, BIN1, PWMA, PWMB, STBY);







void setup() {

  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
}