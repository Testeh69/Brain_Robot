#include <Arduino.h>
#include "ultrasonic/ultrasonic.hpp"
#include <Servo.h>
#include "serial.cpp"
#include "structure/buffer_circulaire.hpp"


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

RingBuffer<DataSend> buffer_send(10);
RingBuffer<DataSend> buffer_receive(10);





void setup() {

  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float distance = (float)sensor.getDistance();
  struct DataSend data = {1, distance};
  buffer_receive.push(data);
  sendData(buffer_receive);

}