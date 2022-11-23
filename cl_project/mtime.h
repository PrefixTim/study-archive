
#ifndef Mtime_h
#define Mtime_h
namespace time {
    struct TimeBcd {
        static constexpr char week_days[8][4] = {"", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
        uint8_t seconds;
        uint8_t minutes;
        uint8_t hours;
        uint8_t week_day;
        uint8_t day;
        uint8_t month;
        uint8_t year;

        bool operator<(TimeBcd const &other) {
            return year < other.year || month < other.month || day < other.day || hours < other.hours || minutes < other.minutes || seconds < other.seconds;
        }

        inline void printSeconds(Print &p) {
            p.print(seconds >> 4 & 0xF);
            p.print(seconds & 0xF);
        }

        inline void printMinutes(Print &p) {
            p.print(minutes >> 4);
            p.print(minutes & 0xF);
        }

        inline void printHours(Print &p) {
            p.print(hours >> 4 & 0xF);
            p.print(hours & 0xF);
        }

        inline void printWeekDay(Print &p) {
            p.print(week_days[week_day]);
        }

        inline void printDay(Print &p) {
            p.print(day >> 4);
            p.print(day & 0xF);
        }

        inline void printMonth(Print &p) {
            p.print(month >> 4);
            p.print(month & 0xF);
        }

        inline void printYear(Print &p) {
            p.print(20);
            p.print(year >> 4);
            p.print(year & 0xF);
        }
    };
} // namespace time
#endif