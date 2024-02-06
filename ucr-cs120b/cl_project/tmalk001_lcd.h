#include <Print.h>
#include <Arduino.h>
#include <stdint.h>
#ifndef LCD_H
#define LCD_H
namespace display {
    class LCD1602REG : public Print {
    public:
        using Print::write;
        LCD1602REG(uint8_t ser, uint8_t rclk, uint8_t srclk);
        void begin();
        
        inline void setCursor(uint8_t col, uint8_t row) {
            command(0x80 | (col + row * 0x40));
        }

        inline void clear() {
            command(0x1);
            delayMicroseconds(1520); // delay 1.52 ms between call for CLear and Home
        }

        inline void home() {
            command(0x2);
            delayMicroseconds(1520); // delay 1.52 ms between call for CLear and Home
        }
        inline void command(uint8_t value) { // delay 37 us between each call (should be fine =P )
            send(value, LOW);
        }

        inline size_t write(uint8_t value) {
            send(value, HIGH);
            return 1;
        }

    private:
        uint8_t _ser, _rclk, _srclk;
        void send(uint8_t value, uint8_t mode);
    };
} // namespace display
#endif