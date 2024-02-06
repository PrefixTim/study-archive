#include "tmalk001_mtime.h"
namespace mtime {
    constexpr char TimeBcd::week_days[8][4];

    bool TimeBcd::operator<(TimeBcd const &other) {
        return year < other.year || month < other.month || day < other.day || isLessTime(other);
    }

    TimeBcd dec_to_bcd(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t week_day, uint8_t day, uint8_t month, uint8_t year) {
        return {dec2bcd(seconds), dec2bcd(minutes), dec2bcd(hours), week_day, dec2bcd(day), dec2bcd(month), dec2bcd(year)};
    }
}
