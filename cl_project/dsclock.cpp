#include "dsclock.h"

namespace mtime {
    void DS1307::begin() {
        Wire.begin();
    }

    void DS1307::writeTime(TimeBcd time) {
        Wire.beginTransmission(0x68);
        Wire.write(0);
        for (auto i : t.arr)
            Wire.write(i);
        Wire.endTransmission();
        t.time = time;
    }

    void DS1307::readTime() {
        Wire.beginTransmission(0x68);
        Wire.write(0);
        Wire.endTransmission();
        Wire.requestFrom(0x68, 7);
        for (auto &i : t.arr) i = Wire.read();
    }

    uint8_t DS1307::getWeekDay() {
        return t.time.week_day;
    }

    bool DS1307::hasPassedFull(TimeBcd time) {
        return time < t.time;
    }
    
    bool DS1307::hasPassedTime(TimeBcd time) {
        return time.isLessTime(time);
    }
    
    DS1307 clock;

    int ClockTick(int state) {
        clock.readTime();
        return state;
    }
}
