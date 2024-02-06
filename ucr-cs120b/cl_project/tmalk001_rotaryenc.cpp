#include "tmalk001_rotaryenc.h"
#include "tmalk001_glue.h"

namespace rotary_encoder {
    RotaryEncoder::RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw) : val(0), pin_clk(clk), pin_dt(dt), pin_sw(sw), pr_btn(0) {}

    void RotaryEncoder::begin(void (*fall_clk)(void), void (*fall_dt)(void)) {
        pinMode(pin_clk, INPUT);
        pinMode(pin_dt, INPUT);
        pinMode(pin_sw, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin_clk), fall_clk, FALLING);
        attachInterrupt(digitalPinToInterrupt(pin_dt), fall_dt, FALLING);
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

    RotaryEncoder rot_enc(2, 3, 4);

    int RE_TickFct(int state) {
        static uint8_t i;
        switch (state) {
        case RE_Start:
            i = 0;
            rot_enc.begin(fall_clk, fall_dt);
        case RE_Enable:
        case RE_Disable:
            state = glue::enable_input ? RE_Enable : RE_Disable;
            break;
        }
        switch (state) {
        case RE_Enable:
            if (++i == 3) {
                i = 0;
                int8_t tmp = rot_enc.clear();
                if (tmp > 0)
                    glue::input_event_queue.push({glue::InputEvent::Inc, (uint8_t)tmp});
                else if (tmp < 0)
                    glue::input_event_queue.push({glue::InputEvent::Dec, (uint8_t)-tmp});
            }
            if (rot_enc.read_sw())
                glue::input_event_queue.push({glue::InputEvent::Press, 1});
            break;
        default:
            break;
        }
        return state;
    }

} // namespace rotary_encoder
