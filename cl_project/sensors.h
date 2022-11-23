#include <stdint.h>

#ifndef Sensors_H
#define Sensors_H
namespace sensors {
class Usonic {
   public:
    Usonic(uint8_t echo_pin, uint8_t trig_pin);
    void begin();
    inline float readDistance();

   private:
    uint8_t echo_pin;
    uint8_t trig_pin;
};

}  // namespace sensors
#endif
