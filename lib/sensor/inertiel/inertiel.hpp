#pragma once
#include <Wire.h>

#ifndef MPU_6050
#define MPU_6050 0x68
#endif
#ifndef ACCEL_ADRESS
#define ACCEL_ADRESS 0x3B
#endif
#ifndef GYRO_ADRESS
#define GYRO_ADRESS 0x43
#endif
#ifndef GYRO_FS_SEL
#define GYRO_FS_SEL 0x1B
#endif
#ifndef ACCEL_FS_SEL
#define ACCEL_FS_SEL 0x1C
#endif
#ifndef TEMP_ADRESS
#define TEMP_ADRESS 0x41
#endif




struct GyroData {
    float x;
    float y;
    float z;
};


struct OrientationData {
    float roll;
    float pitch;
    float yaw;
};


struct AccelData{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct SpeedData{
    int16_t x;
    int16_t y;
    int16_t z;
};


struct PositionData{
    int16_t x;
    int16_t y;
    int16_t z;
};

class InertielSensor{

    private:
    // Paramètres de configuration du MPU6050

    GyroData gyroData;
    OrientationData rotationData;
    

    AccelData accelData;
    SpeedData speedData;
    PositionData positionData;

    unsigned long previousTimeGyro = millis();
    unsigned long previousTimeAccel = millis();

    float temp;
    float gyroScale = 131.0f;
    float accelScale = 16384.0f; 

    // Méthodes privées pour lire et écrire les registres du MPU6050

    int16_t readRegister(uint8_t reg, uint8_t deviceAddress, bool isDoubleAddress) {
        // Lire un octet depuis le registre spécifié de l'appareil I2C
        Wire.beginTransmission(deviceAddress);
        Wire.write(reg);
        Wire.endTransmission(false);
        if (isDoubleAddress) {
            Wire.requestFrom(deviceAddress, (uint8_t)2);
            int16_t value = Wire.read() << 8;
            value |= Wire.read();
            return value;
        } else {
            Wire.requestFrom(deviceAddress, (uint8_t)1);
            int16_t value = Wire.read();
            return value;
        }
    }
    
    void writeRegister(uint8_t reg, uint8_t value, uint8_t deviceAddress) {
        // Écrire un octet dans le registre spécifié de l'appareil I2C
        Wire.beginTransmission(deviceAddress);
        Wire.write(reg);
        Wire.write(value);
        Wire.endTransmission();
    }

    


    float getGyroScale(uint8_t fsSel) {
        // Retourner le facteur de conversion pour les données gyroscopiques en fonction du fsSel
        switch (fsSel) {
            case 0: return 131.0;   // ±250 °/s
            case 1: return 65.5;    // ±500 °/s
            case 2: return 32.8;    // ±1000 °/s
            case 3: return 16.4;    // ±2000 °/s
            default: return 131.0;  // Valeur par défaut (±250 °/s)
        }
        
    }


    float getAccelScale(uint8_t fsSel) {
        // Retourner le facteur de conversion pour les données d'accélération en fonction du fsSel
        switch (fsSel) {
            case 0: return 16384.0;  // ±2 g
            case 1: return 8192.0;   // ±4 g
            case 2: return 4096.0;   // ±8 g
            case 3: return 2048.0;   // ±16 g
            default: return 16384.0; // Valeur par défaut (±2 g)
        }
    }

    public:


    void initialize() {
        // Initialiser le MPU6050 (par exemple, en configurant les registres nécessaires)
        writeRegister(0x6B, 0x00, MPU_6050); // Sortir du mode veille
    }

    void setConfigGyro(uint8_t fsSel) {
        // Configurer le gyroscope en fonction du fsSel et retourner le facteur de conversion
        writeRegister(GYRO_FS_SEL, fsSel << 3, MPU_6050);
        gyroScale = getGyroScale(fsSel);
    }


    void setConfigAccel(uint8_t fsSel) {
        // Configurer l'accéléromètre en fonction du fsSel et retourner le facteur de conversion
        writeRegister(ACCEL_FS_SEL, fsSel << 3, MPU_6050);
        accelScale = getAccelScale(fsSel);
    }


    GyroData getGyro(){
        // Lire les données du gyroscope, les convertir en degrés par seconde et les retourner
        gyroData.x = (float)readRegister(GYRO_ADRESS, MPU_6050, true) / gyroScale;
        gyroData.y = (float)readRegister(GYRO_ADRESS + 2, MPU_6050, true) / gyroScale;
        gyroData.z = (float)readRegister(GYRO_ADRESS + 4, MPU_6050, true) / gyroScale;
      
        return gyroData;
    }

    OrientationData getOrientation(float deltaTime){
        rotationData.roll += gyroData.x * deltaTime;
        rotationData.pitch += gyroData.y * deltaTime;
        rotationData.yaw += gyroData.z * deltaTime;
        return rotationData;

    }


    AccelData getAccel(){
        // Lire les données de l'accéléromètre, les convertir en g et les retourner
        accelData.x = (int16_t)readRegister(ACCEL_ADRESS, MPU_6050, true) / accelScale;
        accelData.y = (int16_t)readRegister(ACCEL_ADRESS + 2, MPU_6050, true) / accelScale;
        accelData.z = (int16_t)readRegister(ACCEL_ADRESS + 4, MPU_6050, true) / accelScale;

        return accelData;
    }


    SpeedData getSpeed(float deltaTime){
        speedData.x += accelData.x * deltaTime;
        speedData.y += accelData.y * deltaTime;
        speedData.z += accelData.z * deltaTime;

        return speedData;
    }

    PositionData getPosition(float deltaTime){
        // Calculer la position en intégrant les données de vitesse et les retourner

        positionData.x += speedData.x * deltaTime;
        positionData.y += speedData.y * deltaTime;
        positionData.z += speedData.z * deltaTime;

        return positionData;
    };

    void StrapDownInertialNavigation(){
        unsigned long currentTime = millis();
        float deltaTimeGyro = (currentTime - previousTimeGyro) / 1000.0; // Convertir en secondes
        float deltaTimeAccel = (currentTime - previousTimeAccel) / 1000.0; // Convertir en secondes
        previousTimeGyro = currentTime;
        previousTimeAccel = currentTime;
        getGyro();
        getOrientation(deltaTimeGyro);
        getAccel();
        getSpeed(deltaTimeAccel);
        getPosition(deltaTimeAccel);

    }

    float getTemp(){
        // Lire la température, la convertir en degrés Celsius et la retourner
        temp = (float)readRegister(TEMP_ADRESS, MPU_6050, true) / 340.0 + 36.53;
        return temp;
    }

};





