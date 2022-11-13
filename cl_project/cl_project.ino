#include <Wire.h>

#include "Print.h"

const char* week_days[8] = {"", "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
struct TimeBcd {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfWeek;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t year;
};

union TimeUnion {
    TimeBcd time;
    uint8_t arr[7];
};

struct DS1307 {
    TimeUnion t;
    void begin() {
        Wire.begin();
    }

    void setTime(TimeBcd _time) {
        Wire.beginTransmission(0x68);
        Wire.write((uint8_t)0x00);
        for (auto i : t.arr) Wire.write(i);
        Wire.endTransmission();
        t.time = _time;
    }

    void getTime() {
        Wire.beginTransmission(0x68);
        Wire.write((uint8_t)0x00);
        Wire.endTransmission();
        Wire.requestFrom(0x68, 7);
        for (auto& i : t.arr) i = Wire.read();
    }
};

class LCD1602REG : public Print {
   public:
    using Print::write;
    LCD1602REG(uint8_t ser, uint8_t rclk, uint8_t srclk) {
        _ser = ser;
        _rclk = rclk;
        _srclk = srclk;
    }
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

LCD1602REG lcd(11, 12, 13);
DS1307 clock;  // define a object of DS1307 class

void setup() {
    Serial.begin(115200);
    lcd.begin();
    clock.begin();
}

void loop() {
    printTime();
    delay(1000);
}

void printTime() {
    lcd.home();
    clock.getTime();
    lcd.print(clock.t.time.hour >> 4 & 0xF);
    lcd.print(clock.t.time.hour & 0xF);
    lcd.print(":");
    lcd.print(clock.t.time.minute >> 4);
    lcd.print(clock.t.time.minute & 0xF);
    lcd.print(":");
    lcd.print(clock.t.time.second >> 4 & 0xF);
    lcd.print(clock.t.time.second & 0xF);
    lcd.print("  ");
    lcd.print(week_days[clock.t.time.dayOfWeek]);
    lcd.setCursor(0, 1);
    lcd.print(clock.t.time.month >> 4);
    lcd.print(clock.t.time.month & 0xF);
    lcd.print("/");
    lcd.print(clock.t.time.dayOfMonth >> 4);
    lcd.print(clock.t.time.dayOfMonth & 0xF);
    lcd.print("/");
    lcd.print(20);
    lcd.print(clock.t.time.year >> 4);
    lcd.print(clock.t.time.year & 0xF);
}
