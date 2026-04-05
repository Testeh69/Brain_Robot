#include <Arduino.h>
#include "ultrasonic/ultrasonic.hpp"
#include "inertiel/inertiel.hpp"
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
InertielSensor inertiel;


RingBuffer<Paquet<float>> buffer_send(32);
RingBuffer<Paquet<float>> buffer_receive(32);





void setup() {
  // Pin du servo si tu l'utilises plus tard
  pinMode(servoPin, OUTPUT);
  
  // Augmenter la vitesse pour le flux de données
  Serial.begin(9600); 
  
  // Initialisation I2C pour le MPU6050
  Wire.begin(); 
  inertiel.initialize();
  inertiel.setConfigGyro(0); 
  inertiel.setConfigAccel(0);
}

void fillBufferWithSensorData(RingBuffer<Paquet<float>>& buffer) {
  // Lecture des capteurs
  GyroData gyro = inertiel.getGyro();
  AccelData accel = inertiel.getAccel();
  float dist = (float)sensor.getDistance();

  // Envoi direct dans le buffer pour éviter les redéclarations
  // Syntaxe : buffer.push( Paquet<type>(id, valeur) )
  buffer.push(Paquet<float>(211, gyro.x));
  buffer.push(Paquet<float>(212, gyro.y));
  buffer.push(Paquet<float>(213, gyro.z));
  
  buffer.push(Paquet<float>(221, accel.x));
  buffer.push(Paquet<float>(222, accel.y));
  buffer.push(Paquet<float>(223, accel.z));
  
  buffer.push(Paquet<float>(3, dist));
}

unsigned long lastMillis = 0;

void loop() {
  // On échantillonne toutes les 50ms (20Hz) pour ne pas saturer le buffer
  if (millis() - lastMillis >= 50) {
    lastMillis = millis();
    fillBufferWithSensorData(buffer_send);
  }

  // On vide le buffer dès que possible
  sendData(buffer_send);
}