#include <Arduino.h>
#include "structure/buffer_circulaire.hpp"



// Map ID des capteurs
/*
1: Ultrasonic sensor
2:
 -- 2.1: Gyroscope
    -- 2.1.1: Gyro X
    -- 2.1.2: Gyro Y
    -- 2.1.3: Gyro Z
 -- 2.2: Accelerometer
    -- 2.2.1: Accel X
    -- 2.2.2: Accel Y
    -- 2.2.3: Accel Z
3: Moteur droit
4: Moteur gauche
*/

template <typename T>
struct Paquet {
    int id;
    T value;
    unsigned long timestamp;

    // Constructeur pour initialiser facilement la structure
    Paquet(int _id, T _val) {
        id = _id;
        value = _val;
        timestamp = millis() / 1000; // Enregistre le temps en secondes au moment de la création
    }
    
    // Constructeur vide (parfois nécessaire pour les tableaux)
    Paquet() : id(0), timestamp(0) {}
};



template <typename T>
void sendData(RingBuffer<T>& buffer) {
    if (!buffer.isEmpty()) {
        T data = buffer.pop();
        Serial.print(data.id);
        Serial.print(": ");
        Serial.print(data.value);
        Serial.print(":");
        Serial.println(data.timestamp);

    }
}


template <typename T>
void receiveData(RingBuffer<T>& buffer) {
    while (Serial.available() > 0) {
        T data = (T)Serial.read();
        buffer.push(data);
    }
}





