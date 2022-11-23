#include <Wire.h>
#include "mtime.h"

#ifndef Dsclock_H
#define Dsclock_H
namespace time {
    struct DS1307 {
        union TimeUnion {
            TimeBcd time;
            uint8_t arr[7];
        };

        TimeUnion t;
        void begin() {
            Wire.begin();
        }

        void writeTime(TimeBcd time) {
            Wire.beginTransmission(0x68);
            Wire.write(0);
            for (auto i : t.arr)
                Wire.write(i);
            Wire.endTransmission();
            t.time = time;
        }

        void readTime() {
            Wire.beginTransmission(0x68);
            Wire.write(0);
            Wire.endTransmission();
            Wire.requestFrom(0x68, 7);
            for (auto &i : t.arr)
                i = Wire.read();
        }

        bool hasPassed(TimeBcd time) {
            return time < t.time;
        }
    };
    
    extern  DS1307 clock;
    int ClockTick(int state);
}
#endif