#include "sensors.h"
#include <Arduino.h>

namespace sensors {
    Usonic::Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void Usonic::begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

} // namespace sensors
