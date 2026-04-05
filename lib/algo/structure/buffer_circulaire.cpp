#include <Arduino.h>
#include "buffer_circulaire.hpp"



   
template <typename T>
RingBuffer<T>::RingBuffer(size_t s) : size(s) {
    buffer = new T[size];
}


template <typename T>
void RingBuffer<T>::push(T value){
            if (!full){
                buffer[write] = value;
                write = (write + 1) % size;
                updateFull();
            }
           
       }

template <typename T>
T RingBuffer<T>::pop(){
    if (write == read && !full) {
        // Buffer vide
        return T(); // ou une autre valeur pour indiquer que le buffer est vide
        }
    T value = buffer[read];
    read = (read + 1) % size;
    full = false;
    return value;                
}

template <typename T>
RingBuffer<T>::~RingBuffer() {
    delete[] buffer;
}

  
        
        
template <typename T>        
void RingBuffer<T>::updateFull() {
    if (write == read && !full) {
        full = true;
        }
    }
        
template <typename T>
bool RingBuffer<T>::isEmpty() {
    return (write == read && !full);
}