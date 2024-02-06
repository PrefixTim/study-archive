#include "tmalk001_output.h"
#include "tmalk001_glue.h"
namespace output {
    int BuzzTickFct(int state) {
        static uint8_t i = 0;
        switch (state) {
        case Buzz_Start:
            pinMode(9, OUTPUT);
            state = Buzz_Wait;
            break;
        case Buzz_Run:
            if (!glue::is_buzz_active()) {
                state = Buzz_Wait;
                // noTone(9);
                digitalWrite(9, 0);

            } else {
                // tone(9, 1000);
                digitalWrite(9, i);
                i ^= 1;
            }
            break;
        case Buzz_Wait:
            if (glue::is_buzz_active()) {
                state = Buzz_Run;
            }
            break;
        }
        return state;
    }

    int LedTickFct(int state) {
                static uint8_t i = 0;
        switch (state) {
        case Led_Start:
            pinMode(8, OUTPUT);
            state = Led_Wait;
            break;
        case Led_Run:
            if (!glue::is_led_active()) {
                state = Led_Wait;
                // noTone(9);
                digitalWrite(8, 0);

            } else {
                // tone(9, 1000);
                digitalWrite(8, i);
                i ^= 1;
            }
            break;
        case Led_Wait:
            if (glue::is_led_active()) {
                state = Led_Run;
            }
            break;
        }
        return state;
    }
} // namespace output
