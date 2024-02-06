#include "tmalk001_lcd.h"

namespace display {
    LCD1602REG::LCD1602REG(uint8_t ser, uint8_t rclk, uint8_t srclk) : _ser(ser), _rclk(rclk), _srclk(srclk) {}
    void LCD1602REG::begin() {
        pinMode(_ser, OUTPUT);
        pinMode(_rclk, OUTPUT);
        pinMode(_srclk, OUTPUT);
        while (millis() < 20)
            ;          // must be at least 15ms since power on
        command(0x38); // Functoin Set: DL=8, N=2, F=5x8
        command(0x0C); // Display ON?OFF: D=1, C=0, B=0
        command(0x06); // Entry mode
        command(0x01); // Clear Display
    }
    
    void LCD1602REG::send(uint8_t value, uint8_t mode) {
            digitalWrite(_rclk, LOW);
            noInterrupts();
            for (int8_t i = 7; i >= 0; i--) {
                digitalWrite(_ser, (value >> i) & 0x1);
                digitalWrite(_srclk, HIGH);
                digitalWrite(_srclk, LOW);
            }
            digitalWrite(_ser, mode);
            digitalWrite(_rclk, HIGH);
            interrupts();
            digitalWrite(_rclk, LOW);
            digitalWrite(_ser, LOW);
        }
} // namespace display
