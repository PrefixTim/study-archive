#include <Wire.h>

#include "Print.h"

#define DS1307_I2C_ADDRESS 0x68

#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6
#define SUN 7

class DS1307 {
   private:
    uint8_t decToBcd(uint8_t val);
    uint8_t bcdToDec(uint8_t val);

   public:
    void begin();
    // void begin(uint16_t SDA ,uint16_t SCL);
    void startClock(void);
    void stopClock(void);
    void setTime(void);
    void getTime(void);
    void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
    void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
    void fillDayOfWeek(uint8_t _dow);
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfWeek;  // day of week, 1 = Monday
    uint8_t dayOfMonth;
    uint8_t month;
    uint16_t year;
};

uint8_t DS1307::decToBcd(uint8_t val) {
    return ((val / 10 * 16) + (val % 10));
}

// Convert binary coded decimal to normal decimal numbers
uint8_t DS1307::bcdToDec(uint8_t val) {
    return ((val / 16 * 10) + (val % 16));
}

void DS1307::begin() {
    Wire.begin();
}

/*Function: The clock timing will start */
void DS1307::startClock(void) {  // set the ClockHalt bit low to start the rtc
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);  // Register 0x00 holds the oscillator start/stop bit
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
    second = Wire.read() & 0x7f;  // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write((uint8_t)second);  // write seconds back and start the clock
    Wire.endTransmission();
}
/*Function: The clock timing will stop */
void DS1307::stopClock(void) {  // set the ClockHalt bit high to stop the rtc
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);  // Register 0x00 holds the oscillator start/stop bit
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
    second = Wire.read() | 0x80;  // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write((uint8_t)second);  // write seconds back and stop the clock
    Wire.endTransmission();
}
/****************************************************************/
/*Function: Read time and date from RTC */
void DS1307::getTime() {
    // Reset the register pointer
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
    // A few of these need masks because certain bits are control bits
    second = bcdToDec(Wire.read() & 0x7f);
    minute = bcdToDec(Wire.read());
    hour = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
    dayOfWeek = bcdToDec(Wire.read());
    dayOfMonth = bcdToDec(Wire.read());
    month = bcdToDec(Wire.read());
    year = bcdToDec(Wire.read());
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void DS1307::setTime() {
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write(decToBcd(second));  // 0 to bit 7 starts the clock
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6
    Wire.write(decToBcd(dayOfWeek));
    Wire.write(decToBcd(dayOfMonth));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.endTransmission();
}
void DS1307::fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second) {
    // assign variables
    hour = _hour;
    minute = _minute;
    second = _second;
}
void DS1307::fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day) {
    year = _year - 2000;
    month = _month;
    dayOfMonth = _day;
}
void DS1307::fillDayOfWeek(uint8_t _dow) {
    dayOfWeek = _dow;
}

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

    // clock.fillByYMD(2022,11,9);//Jan 19,2013
    // clock.fillByHMS(23,53,20);//15:28 30"
    // clock.fillDayOfWeek(WED);//Saturday
    // clock.setTime();//write time to the RTC chip

}

void loop() {
    printTime();
    delay(1000);
}

/*Function: Display time on the serial monitor*/
void printTime() {
    lcd.home();
    clock.getTime();
    lcd.print(clock.hour, DEC);
    lcd.print(":");
    lcd.print(clock.minute, DEC);
    lcd.print(":");
    lcd.print(clock.second, DEC);
    lcd.print("  ");
    lcd.print(clock.month, DEC);
    lcd.print("/");
    lcd.print(clock.dayOfMonth, DEC);
    lcd.print("/");
    lcd.print(clock.year + 2000, DEC);
    lcd.setCursor(0, 1);
    lcd.print(clock.dayOfMonth);
    lcd.print("*");
    switch (clock.dayOfWeek)  // Friendly printout the weekday
    {
        case MON:
            lcd.print("MON");
            break;
        case TUE:
            lcd.print("TUE");
            break;
        case WED:
            lcd.print("WED");
            break;
        case THU:
            lcd.print("THU");
            break;
        case FRI:
            lcd.print("FRI");
            break;
        case SAT:
            lcd.print("SAT");
            break;
        case SUN:
            lcd.print("SUN");
            break;
        default:
            lcd.print("FUCK");
            break;
    }
}
