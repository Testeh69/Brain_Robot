#include "motordriver.hpp"
#include <Arduino.h>




// === PINS DRIVER TB6612 ===
MotorDriver::MotorDriver(int AIN1, int BIN1, int PWMA, int PWMB, int STBY) {
  this->AIN1 = AIN1;
  this->BIN1 = BIN1;
  this->PWMA = PWMA;
  this->PWMB = PWMB;
  this->STBY = STBY;
  pinMode(AIN1, OUTPUT);// Moteurs DROITS
  pinMode(BIN1, OUTPUT);// Moteurs GAUCHES
  pinMode(PWMA, OUTPUT);// PWM vitesse moteur DROIT
  pinMode(PWMB, OUTPUT);// PWM vitesse moteur GAUCHE
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH); // Active le TB6612
}

// Méthodes controle robot
void MotorDriver::Avancer(int speed, MPU6050& mpu) {
  // PID control to maintain straight movement
  const float consigne_angle = 0; 
  
  float angle_actuel = mpu.get_angle(2); 
  float erreur = consigne_angle - angle_actuel; 
  this->integral += erreur * dt;
  float derivative = (erreur - previous_error) / dt;
  previous_error = erreur;
  float correction = this->Kp * erreur + this->Ki * integral + this->Kd * derivative;
  int pwmA = this->clampSpeed(speed + correction + this->compensationMecanique);
  int pwmB = this->clampSpeed(speed - correction - this->compensationMecanique); 
  this->setMotorA(pwmA);
  this->setMotorB(pwmB);
}

void MotorDriver::Reculer(int speed, MPU6050& mpu) {
  const float consigne_angle = 0;
  float angle_actuel = mpu.get_angle(2);
  float erreur = consigne_angle - angle_actuel;
  this->integral += erreur * dt;
  float derivative = (erreur - previous_error) / dt;
  previous_error = erreur;
  float correction = this->Kp * erreur + this->Ki * integral + this->Kd * derivative;
  int pwmA = this->clampSpeed(speed + correction + this->compensationMecanique);
  int pwmB = this->clampSpeed(speed - correction - this->compensationMecanique);
  this->setMotorA(-pwmA);
  this->setMotorB(-pwmB);

}


void MotorDriver::TournerGauche(float consigne_angle , MPU6050& mpu) {
    const int speed = 50;
    float angle_depart = mpu.get_angle(2);
    float delta_angle = 0.0f;
    while (delta_angle < consigne_angle) {
        setMotorA(speed);
        setMotorB(-speed);
        float angle_actuel = mpu.get_angle(2);
        delta_angle = angle_actuel - angle_depart;
    }

    // stop motors
    setMotorA(0);
    setMotorB(0);
}

void MotorDriver::TournerDroite(float consigne_angle , MPU6050& mpu) {
    const int speed = 50;
    float angle_depart = mpu.get_angle(2);
    float delta_angle = 0.0f;
    while (delta_angle < consigne_angle) {
        setMotorA(-speed);
        setMotorB(speed);
        float angle_actuel = 360.0f - mpu.get_angle(2);
        delta_angle = angle_actuel - angle_depart;
    }
    // stop motors
    setMotorA(0);
    setMotorB(0);
}



// Méthodes pour contrôler les moteurs
void MotorDriver::setMotorA(int speed) {
  digitalWrite(AIN1, speed > 0 ? HIGH : LOW);
  analogWrite(PWMA, abs(speed));
}


void MotorDriver::setMotorB(int speed) {
  digitalWrite(BIN1, speed > 0 ? HIGH : LOW);
  analogWrite(PWMB, abs(speed));
}

int MotorDriver::clampSpeed(int speed) {
  if (speed > 255) return 255;
  if (speed < -255) return -255;
  return speed;
}