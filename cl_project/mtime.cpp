#include  "mtime.h"
namespace mtime {
    constexpr char TimeBcd::week_days[8][4];
    
    bool TimeBcd::operator<(TimeBcd const &other) {
        return year < other.year || month < other.month || day < other.day || isLessTime(other);
    }
    
    inline bool TimeBcd::isLessTime(TimeBcd const &other) {
        return hours < other.hours || minutes < other.minutes || seconds < other.seconds;
    }

    inline void TimeBcd::printSeconds(const Print &p) {
        p.print(seconds >> 4 & 0xF);
        p.print(seconds & 0xF);
    }

    inline void TimeBcd::printMinutes(const Print &p) {
        p.print(minutes >> 4);
        p.print(minutes & 0xF);
    }

    inline void TimeBcd::printHours(const Print &p) {
        p.print(hours >> 4 & 0xF);
        p.print(hours & 0xF);
    }

    inline void TimeBcd::printWeekDay(const Print &p) {
        p.print(week_days[week_day]);
    }

    inline void TimeBcd::printDay(const Print &p) {
        p.print(day >> 4);
        p.print(day & 0xF);
    }

    inline void TimeBcd::printMonth(const Print &p) {
        p.print(month >> 4);
        p.print(month & 0xF);
    }

    inline void TimeBcd::printYear(const Print &p) {
        p.print(20);
        p.print(year >> 4);
        p.print(year & 0xF);
    }
}
