#include <Arduino.h>
#include "rotaryenc.h"
#include "glue.h"

namespace rotary_encoder {
    RotaryEncoder::RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw) : val(0), pin_clk(clk), pin_dt(dt), pin_sw(sw), pr_btn(0) {}

    void RotaryEncoder::begin(void (*fall_clk)(void), void (*fall_dt)(void)) {
        pinMode(pin_clk, INPUT);
        pinMode(pin_dt, INPUT);
        pinMode(pin_sw, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin_clk), fall_clk, FALLING);
        attachInterrupt(digitalPinToInterrupt(pin_dt), fall_dt, FALLING);
    }

    inline uint8_t RotaryEncoder::read_clk() {
        return digitalRead(pin_clk);
    }

    inline uint8_t RotaryEncoder::read_dt() {
        return digitalRead(pin_dt);
    }

    inline uint8_t RotaryEncoder::read_sw() {
        uint8_t tmp = digitalRead(pin_sw);
        uint8_t res = pr_btn & !tmp;
        pr_btn = tmp;
        return res;
    }

    inline void RotaryEncoder::add(int8_t dir) {
        val += dir;
    }

    int8_t RotaryEncoder::clear() {
        noInterrupts();
        int8_t tmp = val;
        val = 0;
        interrupts();
        return tmp;
    }

    void fall_clk() {
        interRotEnc(0, rot_enc.read_dt());
    }

    void fall_dt() {
        interRotEnc(rot_enc.read_clk(), 0);
    }

    inline void interRotEnc(uint8_t clk, uint8_t dt) {
        static short dir = 0;
        if (clk ^ dt) {
            dir = dt ? -1 : 1;
        } else if (!(clk & dt)) {
            rot_enc.add(dir);
            dir = 0;
        }
    }

    RotaryEncoder rot_enc(2, 3, 4);

    int RE_TickFct(int state) {
        int8_t tmp = rot_enc.clear();
        if (tmp > 0)
            glue::input_event_queue.push({glue::InputEvent::Inc, (uint8_t)tmp});
        else if (tmp < 0)
            glue::input_event_queue.push({glue::InputEvent::Dec, (uint8_t)-tmp});
        return state;
    }

    int RE_Btn_TickFct(int state) {
        uint8_t btn = rot_enc.read_sw();
        if (btn) glue::input_event_queue.push({glue::InputEvent::Press, 1});
        return state;
    }

}  // namespace rotary_encoder
