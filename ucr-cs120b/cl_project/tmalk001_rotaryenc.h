#include <Arduino.h>
#include <stdint.h>

#ifndef RotaryEncoder_h
#define RotaryEncoder_h
namespace rotary_encoder {

    void fall_clk();
    void fall_dt();

    struct RotaryEncoder {
        RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw);
        void begin(void (*fall_clk)(void), void (*fall_dt)(void));
        
        inline uint8_t read_clk() {
            return digitalRead(pin_clk);
        }

        inline uint8_t read_dt() {
            return digitalRead(pin_dt);
        }

        inline uint8_t read_sw() {
            uint8_t tmp = digitalRead(pin_sw);
            uint8_t res = pr_btn & !tmp;
            pr_btn = tmp;
            return res;
        }

        inline void add(int8_t dir) {
            val += dir;
        }
        int8_t clear();

        uint8_t pr_btn;
        uint8_t val;
        uint8_t pin_clk;
        uint8_t pin_dt;
        uint8_t pin_sw;
    };
    extern RotaryEncoder rot_enc;

    inline void interRotEnc(uint8_t clk, uint8_t dt) {
        static short dir = 0;
        if (clk ^ dt) {
            dir = dt ? -1 : 1;
        } else if (!(clk & dt)) {
            rot_enc.add(dir);
            dir = 0;
        }
    }

    void fall_clk();
    void fall_dt();

    enum RE_State { RE_Start,
                    RE_Enable,
                    RE_Disable };
    int RE_TickFct(int state);
} // namespace rotary_encoder
#endif
