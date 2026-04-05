#include <Arduino.h>
#include "structure/buffer_circulaire.hpp"



// Map ID des capteurs
/*

1: Ultrasonic sensor
2: Centrale inertielle
3: Moteur droit
4: Moteur gauche
*/

struct DataSend {
    int id;
    float value;
};



template <typename T>
void sendData(RingBuffer<T>& buffer) {
    if (!buffer.isEmpty()) {
        T data = buffer.pop();
        Serial.println(data.id);
        Serial.println(data.value);
    }
}


template <typename T>
void receiveData(RingBuffer<T>& buffer) {
    while (Serial.available() > 0) {
        T data = (T)Serial.read();
        buffer.push(data);
    }
}





