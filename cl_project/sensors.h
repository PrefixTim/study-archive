#include <stdint.h>
#include <Arduino.h>

#ifndef Sensors_H
#define Sensors_H
namespace sensors {
    class Usonic {
    public:
        Usonic(uint8_t echo_pin, uint8_t trig_pin);
        void begin();
        inline float readDistance() {
            noInterrupts();
            digitalWrite(trig_pin, LOW);
            delayMicroseconds(2);
            digitalWrite(trig_pin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trig_pin, LOW);
            float res = pulseIn(echo_pin, HIGH) * 17 / 1000;
            interrupts();
            return res;
        }

    private:
        uint8_t echo_pin;
        uint8_t trig_pin;
    };

} // namespace sensors
#endif
