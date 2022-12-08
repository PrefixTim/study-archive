#include "dsclock.h"
#include "glue.h"
namespace mtime {
    void DS1307::begin() {
        Wire.begin(0x68);
    }

    void DS1307::writeTime(TimeBcd time) {
        t.time = time;
        Wire.beginTransmission(0x68);
        Wire.write(0);
        for (auto i : t.arr)
            Wire.write(i);
        Wire.endTransmission();
    }

    void DS1307::readTime() {
        Wire.beginTransmission(0x68);
        Wire.write(0);
        Wire.endTransmission();
        Wire.requestFrom(0x68, 7);
        for (auto &i : t.arr)
            i = Wire.read();
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
        switch (state) {
        case Clock_Start:
            clock.begin();
            state = Clock_Run;
            break;
        case Clock_Run:
            clock.readTime();
            if (!glue::set_time_queue.is_empty()) {
                state = Clock_Change;
            }
            break;
        case Clock_Change:
            auto tmp = glue::set_time_queue.pop();
            clock.writeTime({
                tmp.s != 128 ? dec2bcd(tmp.s) : clock.t.time.seconds,
                tmp.m != 128 ? dec2bcd(tmp.m) : clock.t.time.minutes,
                tmp.h != 128 ? dec2bcd(tmp.h) : clock.t.time.hours,
                clock.t.time.week_day,
                tmp.d != 128 ? dec2bcd(tmp.d) : clock.t.time.day,
                tmp.n != 128 ? dec2bcd(tmp.n) : clock.t.time.month,
                tmp.y != 128 ? dec2bcd(tmp.y) : clock.t.time.year,

            });
            state = Clock_Run;
            break;
        }
        return state;
    }
}
