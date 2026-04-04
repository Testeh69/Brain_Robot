


class UltrasonicSensor {
    public:
    UltrasonicSensor(int trigPin, int echoPin);
    int getDistance();
    void displayDistance();
    
    private:
    int trigPin;
    int echoPin;
    int distance;
};