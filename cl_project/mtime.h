#include <stdint.h>
#include <Print.h>

#ifndef Mtime_h
#define Mtime_h
namespace mtime {
    struct TimeBcd {
        static constexpr char week_days[8][4] = {"", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        uint8_t week_day;
        uint8_t day;
        uint8_t month;
        uint8_t year;

        bool operator<(TimeBcd const &other);
        bool isLessTime(TimeBcd const &other);
        inline void printSeconds(Print &p);
        inline void printMinutes(Print &p);
        inline void printHours(Print &p);
        inline void printWeekDay(Print &p);
        inline void printDay(Print &p);
        inline void printMonth(Print &p);
        inline void printYear(Print &p);
    };
} // namespace time
#endif
