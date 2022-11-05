// #include <Wire.h>

// #ifndef __DS1307_H__
// #define __DS1307_H__


// #define DS1307_I2C_ADDRESS 0x68

// #define MON 1
// #define TUE 2
// #define WED 3
// #define THU 4
// #define FRI 5
// #define SAT 6
// #define SUN 7

// class DS1307 {
//   private:
//     uint8_t decToBcd(uint8_t val);
//     uint8_t bcdToDec(uint8_t val);

//   public:
//     void begin();
//     // void begin(uint16_t SDA ,uint16_t SCL);
//     void startClock(void);
//     void stopClock(void);
//     void setTime(void);
//     void getTime(void);
//     void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
//     void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
//     void fillDayOfWeek(uint8_t _dow);
//     uint8_t second;
//     uint8_t minute;
//     uint8_t hour;
//     uint8_t dayOfWeek;// day of week, 1 = Monday
//     uint8_t dayOfMonth;
//     uint8_t month;
//     uint16_t year;
// };

// #endif


// uint8_t DS1307::decToBcd(uint8_t val) {
//     return ((val / 10 * 16) + (val % 10));
// }

// //Convert binary coded decimal to normal decimal numbers
// uint8_t DS1307::bcdToDec(uint8_t val) {
//     return ((val / 16 * 10) + (val % 16));
// }

// void DS1307::begin() {
//     Wire.begin();
// }

// // /**
// //  * \brief          The functio to start the I2C port with specified pins
// //  *                 
// //  * \param SDA      The pin number which is used as SDA pin
// //  * \param SCL      The pin number which is used as SCL pin
// //  *
// //  */
// // void DS1307::begin(uint16_t SDA ,uint16_t SCL) {
// //     Wire.begin(SDA,SCL);
// // }

// /*Function: The clock timing will start */
// void DS1307::startClock(void) {      // set the ClockHalt bit low to start the rtc
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
//     Wire.endTransmission();
//     Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
//     second = Wire.read() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);
//     Wire.write((uint8_t)second);                    // write seconds back and start the clock
//     Wire.endTransmission();
// }
// /*Function: The clock timing will stop */
// void DS1307::stopClock(void) {       // set the ClockHalt bit high to stop the rtc
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
//     Wire.endTransmission();
//     Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
//     second = Wire.read() | 0x80;       // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);
//     Wire.write((uint8_t)second);                    // write seconds back and stop the clock
//     Wire.endTransmission();
// }
// /****************************************************************/
// /*Function: Read time and date from RTC */
// void DS1307::getTime() {
//     // Reset the register pointer
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);
//     Wire.endTransmission();
//     Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
//     // A few of these need masks because certain bits are control bits
//     second     = bcdToDec(Wire.read() & 0x7f);
//     minute     = bcdToDec(Wire.read());
//     hour       = bcdToDec(Wire.read() & 0x3f);// Need to change this if 12 hour am/pm
//     dayOfWeek  = bcdToDec(Wire.read());
//     dayOfMonth = bcdToDec(Wire.read());
//     month      = bcdToDec(Wire.read());
//     year       = bcdToDec(Wire.read());
// }
// /*******************************************************************/
// /*Frunction: Write the time that includes the date to the RTC chip */
// void DS1307::setTime() {
//     Wire.beginTransmission(DS1307_I2C_ADDRESS);
//     Wire.write((uint8_t)0x00);
//     Wire.write(decToBcd(second));// 0 to bit 7 starts the clock
//     Wire.write(decToBcd(minute));
//     Wire.write(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6
//     Wire.write(decToBcd(dayOfWeek));
//     Wire.write(decToBcd(dayOfMonth));
//     Wire.write(decToBcd(month));
//     Wire.write(decToBcd(year));
//     Wire.endTransmission();
// }
// void DS1307::fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second) {
//     // assign variables
//     hour = _hour;
//     minute = _minute;
//     second = _second;
// }
// void DS1307::fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day) {
//     year = _year - 2000;
//     month = _month;
//     dayOfMonth = _day;
// }
// void DS1307::fillDayOfWeek(uint8_t _dow) {
//     dayOfWeek = _dow;
// }




// DS1307 clock;//define a object of DS1307 class
// void setup()
// {
//    Serial.begin(115200);

//     Serial.println("Begin1");
//    clock.begin();

//     Serial.println("Begin2");
// //    clock.fillByYMD(2013,1,19);//Jan 19,2013

// //     Serial.println("Begin3");
// //    clock.fillByHMS(15,28,30);//15:28 30"

// //     Serial.println("Begin4");
// //    clock.fillDayOfWeek(SAT);//Saturday

// //     Serial.println("Begin5");
//    clock.setTime();//write time to the RTC chip

// //     Serial.println("Begin6");
// }
// void loop()
// {
//     Serial.println("Begin");
//    printTime();
//    delay(500);
// }
//    /*Function: Display time on the serial monitor*/
// void printTime()
// {
//    clock.getTime();
//    Serial.print(clock.hour, DEC);
//    Serial.print(":");
//    Serial.print(clock.minute, DEC);
//    Serial.print(":");
//    Serial.print(clock.second, DEC);
//    Serial.print("  ");
//    Serial.print(clock.month, DEC);
//    Serial.print("/");
//    Serial.print(clock.dayOfMonth, DEC);
//    Serial.print("/");
//    Serial.print(clock.year+2000, DEC);
//    Serial.print(" ");
//    Serial.print(clock.dayOfMonth);
//    Serial.print("*");
//    switch (clock.dayOfWeek)// Friendly printout the weekday
//    {
//        case MON:
//        Serial.print("MON");
//        break;
//        case TUE:
//        Serial.print("TUE");
//        break;
//        case WED:
//        Serial.print("WED");
//        break;
//        case THU:
//        Serial.print("THU");
//        break;
//        case FRI:
//        Serial.print("FRI");
//        break;
//        case SAT:
//        Serial.print("SAT");
//        break;
//        case SUN:
//        Serial.print("SUN");
//        break;
//    }
//    Serial.println(" ");
// }
#include<LiquidCrystal.h>
#define SER 11
#define RCLK 12
#define SRCLK 13
struct LCD16 : public Print {
    virtual size_t write(uint8_t);
    void command(uint8_t);
  
    using Print::write;
    void send(uint8_t, uint8_t);
};
inline void LCD16::command(uint8_t value) {
  send(value, LOW);
}
void LCD16::send(uint8_t value, uint8_t mode) {
    short _rs_pin = 9;
   digitalWrite(_rs_pin, mode);

    for (int8_t i = 7; i >= 0; i--) {
        digitalWrite(SER, (value >> i) & 0x1);
        digitalWrite(SRCLK, HIGH);
        delayMicroseconds(1);
        digitalWrite(SRCLK, LOW);
    }
    digitalWrite(RCLK, HIGH);
        delayMicroseconds(1);
    digitalWrite(RCLK, LOW);
  digitalWrite(10, LOW);
  delayMicroseconds(1);    
  digitalWrite(10, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  digitalWrite(10, LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}
inline size_t LCD16::write(uint8_t value) {
  send(value, HIGH);
    return 1; // assume sucess
}


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LCD16 lcd;

#include "Print.h"

void setup() {
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(SER, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(SRCLK, OUTPUT);
    delayMicroseconds(50000); 
    // Send function set command sequence
    lcd.command(LCD_FUNCTIONSET | 0x18);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    lcd.command(LCD_FUNCTIONSET | 0x18);
    delayMicroseconds(150);

    // third go
    lcd.command(LCD_FUNCTIONSET | 0x18);
  
  // finally, set # lines, font size, etc.
  lcd.command(LCD_FUNCTIONSET | 0x18);  

  // turn the display on with no cursor or blinking default
  int _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
   _displaycontrol |= LCD_DISPLAYON;
  lcd.command(LCD_DISPLAYCONTROL | _displaycontrol);

  // clear it off
  lcd.command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
  // Initialize to default text direction (for romance languages)
  // set the entry mode
  lcd.command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
Serial.begin(115200);
    // lcd.begin(16, 2);
}

void loop() {
    if (Serial.available() > 0) {
    // read the incoming byte:
     
lcd.print((char)Serial.read());
  }

    
    // uint8_t val = 0xAA;
    // for (uint8_t i = 0; i < 8; i++) {
    //     digitalWrite(SER, (val >> i) & 0x1);
    //     digitalWrite(SRCLK, HIGH);
    //     digitalWrite(SRCLK, LOW);
    // }
    // digitalWrite(RCLK, HIGH);
    // digitalWrite(RCLK, LOW);
    // delay(1000);
}