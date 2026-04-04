#include "ultrasonic.hpp"
#include <Arduino.h>

UltrasonicSensor::UltrasonicSensor(int trigPin, int echoPin){
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}



int UltrasonicSensor::getDistance() {
  // Envoie une impulsion de 10 microsecondes sur le pin trig
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Mesure la durée de l'impulsion sur le pin echo
  long duration = pulseIn(echoPin, HIGH);

  // Calcule la distance en centimètres (vitesse du son = 343 m/s)
  int distance = duration * 0.034 / 2;
  this -> distance = distance;
  delay(60);
  if (distance <= 0 || distance > 400) {
    return -1; // Retourne -1 si la distance est hors de portée
  }
  else  {
    return distance;
  }
}


void UltrasonicSensor::displayDistance() {
  
  if (this->distance == -1) {
    Serial.println("Distance hors de portée");
  } else {
    Serial.print("Distance: ");
    Serial.print(this->distance);
    Serial.println(" cm");
  }
}


