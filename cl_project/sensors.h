#ifndef Sensors_H
#define Sensors_H
namespace sensors {
class Usonic {
   public:
    Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

    inline float readDistance() {
        digitalWrite(trig_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig_pin, LOW);
        return pulseIn(echo_pin, HIGH) * 17 / 1000;
    }

   private:
    uint8_t echo_pin;
    uint8_t trig_pin;
};

}  // namespace sensors
#endif
