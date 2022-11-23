#include <stdint.h>

#ifndef RotaryEncoder_h
#define RotaryEncoder_h
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


void fall_clk();
void fall_dt();
inline void interRotEnc(uint8_t clk, uint8_t dt) ;

extern RotaryEncoder rot_enc;


int RE_TickFct(int state);
int RE_Btn_TickFct(int state);
}  // namespace rotary_encoder
#endif
