#include <Arduino.h>
#include "structure/buffer_circulaire.hpp"



#ifndef RX
#define RX 0
#endif

#ifndef TX
#define TX 1
#endif

template <typename T>
void sendData(RingBuffer<T>& buffer) {
    if (!buffer.isEmpty()) {
        T data = buffer.pop();
        Serial.println(data);
    }
}


template <typename T>
void receiveData(RingBuffer<T>& buffer) {
    T data = digitalRead(RX); 
    buffer.push(data);
}





