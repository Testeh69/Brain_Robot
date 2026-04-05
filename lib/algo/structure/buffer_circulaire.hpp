#pragma once

template <typename T>
class RingBuffer {
    public:
       RingBuffer(size_t s); 

       void push(T value);
       T pop();

       bool isEmpty();

       ~RingBuffer();


       private:
        T* buffer; // Définir la taille d'un tableau       
        size_t size;         
        size_t write = 0;         
        size_t read = 0;         
        bool full = false;
        
        
        void updateFull();


};