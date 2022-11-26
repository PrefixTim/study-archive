#include <Wire.h>
#include <stdint.h>
#include "mtime.h"
#include <Arduino.h>

#ifndef Dsclock_H
#define Dsclock_H
namespace mtime {
    struct DS1307 {
        union TimeUnion {
            TimeBcd time;
            uint8_t arr[7];
        };

        TimeUnion t;
        void begin();
        void writeTime(TimeBcd time);
        void readTime();
        uint8_t getWeekDay();
        bool hasPassedFull(TimeBcd time);
        bool hasPassedTime(TimeBcd time);
    };
    
    extern DS1307 clock;
    enum Clock_State {Clock_Start, Clock_Run, CLock_Change};
    int ClockTick(int state);
}
#endif
