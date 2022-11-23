#include <Arduino.h>
#include "sensors.h"

namespace sensors {
    Usonic::Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void Usonic::begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

    inline float Usonic::readDistance() {
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
}  // namespace sensors
