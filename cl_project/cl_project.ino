#include <Print.h>
#include <Wire.h>

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
constexpr char TimeBcd::week_days[8][4];

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
        for (auto i : t.arr) Wire.write(i);
        Wire.endTransmission();
        t.time = time;
    }

    void readTime() {
        Wire.beginTransmission(0x68);
        Wire.write(0);
        Wire.endTransmission();
        Wire.requestFrom(0x68, 7);
        for (auto &i : t.arr) i = Wire.read();
    }

    bool hasPassed(TimeBcd time) {
        return time < t.time;
    }
};

class LCD1602REG : public Print {
   public:
    using Print::write;
    LCD1602REG(uint8_t ser, uint8_t rclk, uint8_t srclk) : _ser(ser), _rclk(rclk), _srclk(srclk) {}
    void begin() {
        pinMode(_ser, OUTPUT);
        pinMode(_rclk, OUTPUT);
        pinMode(_srclk, OUTPUT);
        while (millis() < 20)
            ;           // must be at least 15ms since power on
        command(0x38);  // Functoin Set: DL=8, N=2, F=5x8
        command(0x0C);  // Display ON?OFF: D=1, C=0, B=0
        command(0x06);  // Entry mode
        command(0x01);  // Clear Display
    }

    inline void setCursor(uint8_t col, uint8_t row) {
        command(0x80 | (col + row * 0x40));
    }

    inline void clear() {
        command(0x1);
        delayMicroseconds(1520);  // delay 1.52 ms between call for CLear and Home
    }

    inline void home() {
        command(0x2);
        delayMicroseconds(1520);  // delay 1.52 ms between call for CLear and Home
    }
    inline void command(uint8_t value) {  // delay 37 us between each call (should be fine =P )
        send(value, LOW);
    }

    inline size_t write(uint8_t value) override {
        send(value, HIGH);
        return 1;
    }

   private:
    uint8_t _ser, _rclk, _srclk;
    inline void send(uint8_t value, uint8_t mode) {
        digitalWrite(_rclk, LOW);
        for (int8_t i = 7; i >= 0; i--) {
            digitalWrite(_ser, (value >> i) & 0x1);
            digitalWrite(_srclk, HIGH);
            digitalWrite(_srclk, LOW);
        }
        digitalWrite(_ser, mode);
        digitalWrite(_rclk, HIGH);
        digitalWrite(_rclk, LOW);
        digitalWrite(_ser, LOW);
    }
};

class Usonic {
   public:
    Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

    inline float readDistance() {
        digitalWrite(trig_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig_pin, LOW);
        return pulseIn(echo_pin, HIGH) * 17 / 1000;
    }

   private:
    uint8_t echo_pin;
    uint8_t trig_pin;
};

struct Task {
    int state;
    uint16_t period;
    uint32_t lastRun;
    int (*TickFct)(int);
    void tick() {
        state = TickFct(state);
        lastRun = millis();
    }
};

LCD1602REG lcd(11, 12, 13);
DS1307 clock;
Usonic usonic(3, 4);

int ClockTick(int state) {
    clock.readTime();
    printTime(clock.t.time);
    return state;
}

namespace all{
    struct Saved {
        TimeBcd turn_on_schedule[7];
        TimeBcd turn_off_schedule[7];
    };

    Saved global_state;

    struct Event {
        enum EventType{Inc, Dec, Press};
        EventType type;
        uint8_t val;
    };


    struct Menu {
        virtual void listen(Event e);
        virtual void printTo(LCD1602REG& p);
        virtual void update(LCD1602REG& p);
    };

    struct DisplayState {
        Menu* point[20];
        uint8_t previous[7];
        uint8_t pr_pos;
        uint8_t current;

        inline Menu* get_current() {
            return point[current];
        }

        inline void open(uint8_t to_open) {
            previous[++pr_pos] = to_open;
            current = to_open;
        }

        inline void close() {
            current = previous[--pr_pos];
        }
    };
    
    DisplayState dp_state;

    struct SetTurnOn {
        int state = 0;
        virtual void listen(Event e) {
            switch (e.type) {
            case e.Press:
                state++;
                if (state == 4) dp_state.close();
                break;
            case e.Inc:
                
                break;
            case e.Dec:
                break;
            }
        }
        virtual void printTo(LCD1602REG& p) {
            p.print("Slct T to Arm Alarm");
            p.setCursor(0, 1);
            p.print("W:* H:**, M:** S:**");
        }
        virtual void update(LCD1602REG& p) {
            p.setCursor(2, 1);
            p.print("");
        }
    };

};

#include <avr/interrupt.h>


void printTime(TimeBcd time) {
    lcd.setCursor(0, 0);
    time.printHours(lcd);
    lcd.print(":");
    time.printMinutes(lcd);
    lcd.print(":");
    time.printSeconds(lcd);
    lcd.print("  ");
    time.printWeekDay(lcd);
    lcd.setCursor(0, 1);
    time.printYear(lcd);
    lcd.print("/");
    time.printMonth(lcd);
    lcd.print("/");
    time.printDay(lcd);
}


class LimitedQueue {
    uint8_t b, e;
    uint8_t size;

};

#define A 2
#define B 3
short res=0;
short dir = 0;
bool a = 0;
bool b = 0;
void interRotEnc() {
    a = digitalRead(A);
    b = digitalRead(B);
    if (a && b) {
        dir = 0;
    } else if (a ^ b) {
        dir = b?-1:1;
    } else {
        res += dir;
        dir = 0;
    }
}

int TickRotaryEncoder(int state) {
    lcd.setCursor(0,0);
    a = digitalRead(A);
    b = digitalRead(B);
    lcd.print(res);
    lcd.print("  ");
}

#define SW 4

Task tasks[]={
    {0, 10, 0, TickRotaryEncoder}
};

void setup() {

	// Set encoder pins as inputs
	pinMode(A,INPUT);
	pinMode(B,INPUT);
	pinMode(SW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), interRotEnc, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), interRotEnc, FALLING);

    Serial.begin(115200);
    lcd.begin();
    clock.begin();
    usonic.begin();
}

void loop() {
    for (auto &task : tasks) if (millis() - task.lastRun >= task.period) task.tick();
}