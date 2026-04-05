#pragma once

template <typename T>
class RingBuffer {
    public:
       RingBuffer(size_t s) {
              size = s;
              buffer = new T[size];
       }; 

       void push(T value){
         if (!full){
                buffer[write] = value;
                write = (write + 1) % size;
                updateFull();
            }
       };
       T pop(){
         if (write == read && !full) {
        // Buffer vide
            return T(); // ou une autre valeur pour indiquer que le buffer est vide
            }
        T value = buffer[read];
        read = (read + 1) % size;
        full = false;
        return value;    
       };

       bool isEmpty(){
              return (write == read && !full);
       };

       ~RingBuffer(){
                delete[] buffer;
       };


       private:
        T* buffer; // Définir la taille d'un tableau       
        size_t size;         
        size_t write = 0;         
        size_t read = 0;         
        bool full = false;
        
        
        void updateFull(){
            if (write == read && !full) {
                full = true;
            }
        };


};