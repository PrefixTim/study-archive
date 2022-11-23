#ifndef Glue_h
#define Glue_h
namespace rotary_encoder {
void fall_clk();
void fall_dt();
void interRotEnc(uint8_t clk, uint8_t dt);
struct RotaryEncoder {
    RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw);
    void begin(void (*fall_clk)(void), void (*fall_dt)(void));
    inline uint8_t read_clk();
    inline uint8_t read_dt();
    inline uint8_t read_sw();
    inline void add(int8_t dir);
    int8_t clear();

    uint8_t pr_btn;
    uint8_t val;
    uint8_t pin_clk;
    uint8_t pin_dt;
    uint8_t pin_sw;
};

RotaryEncoder rot_enc(2, 3, 4);

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

int RE_TickFct(int state) {
    int8_t tmp = rot_enc.clear();
    if (tmp > 0)
        event_queue.push({Event::Inc, (uint8_t)tmp});
    else if (tmp < 0)
        event_queue.push({Event::Dec, (uint8_t)-tmp});
    return state;
}

int RE_Btn_TickFct(int state) {
    uint8_t btn = rot_enc.read_sw();
    if (btn) event_queue.push({Event::Press, 1});
    return state;
}

}  // namespace rotary_encoder
#endif
