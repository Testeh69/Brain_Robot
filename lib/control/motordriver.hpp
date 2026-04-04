#pragma once

class MPU6050;  // forward declaration

class MotorDriver {


    public:
    MotorDriver(int AIN1, int BIN1, int PWMA, int PWMB, int STBY);
    void Avancer(int speed, MPU6050& mpu);
    void Reculer(int speed, MPU6050& mpu);
    void TournerGauche(float consigne_angle , MPU6050& mpu);
    void TournerDroite(float consigne_angle , MPU6050& mpu);


    private:
    
    // Pins de contrôle pour les moteurs et cartes
    int AIN1;
    int BIN1;
    int PWMA;
    int PWMB;
    int STBY;

    // Variable pour PID
    const float Kp = 1.5; // gain proportionnel pour la correction
    const float Ki = 0.7; // gain intégral pour la correction
    const float Kd = 0.1; // gain dérivé pour la correction
    const float compensationMecanique = 3; // variabl
    float integral = 0;
    float previous_error = 0;
    float dt = 0.1;


    // Méthodes pour contrôler les moteurs
    void setMotorA(int speed);
    void setMotorB(int speed);
    int clampSpeed(int speed);

};



