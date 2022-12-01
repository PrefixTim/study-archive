#include "glue.h"
#include <Arduino.h>
#include <IRremote.h>

#ifndef Remote_h
#define Remote_h
namespace remote {

#define BTN_EQ 0x19
#define BTN_ST_REPT 0xD
#define BTN_FUNC_STOP 0x47
#define BTN_ON_OFF 0x45
#define BTN_VOL_UP 0x46
#define BTN_VOL_DOWN 0x15
#define BTN_UP 0x9
#define BTN_DOWN 0x7
#define BTN_LEFT 0x44
#define BTN_RIGHT 0x43
#define BTN_PLAY 0x40
#define BTN_9 0x4A // 112
#define BTN_8 0x52 // 82
#define BTN_7 0x42 // 102
#define BTN_6 0x5A // 132
#define BTN_5 0x1C // 28
#define BTN_4 0x8  // 10
#define BTN_3 0x5E // 94
#define BTN_2 0x18 // 24
#define BTN_1 0xC  // 12
#define BTN_0 0x16 // 22

    IRrecv irrecv(7);
    enum Re_State { Re_Start,
                    Re_Run,
                    Re_Delay };
    int RemoteTickFct(int state) {
        static uint32_t time;
        switch (state) {
        case Re_Start:
            state = Re_Run;
            time = millis();
            irrecv.enableIRIn();
            break;
        case Re_Run:
            if (!irrecv.decode())
                break;
            if (irrecv.decodedIRData.command != 0 && (time - millis()) >= 450) {
                time = millis();
                switch (irrecv.decodedIRData.command) {
                case BTN_LEFT:
                    glue::input_event_queue.push({glue::InputEvent::Dec, 1});
                    break;
                case BTN_RIGHT:
                    glue::input_event_queue.push({glue::InputEvent::Inc, 1});
                    break;
                case BTN_PLAY:
                    glue::input_event_queue.push({glue::InputEvent::Press, 1});
                    break;
                case BTN_0:
                case BTN_1:
                case BTN_2:
                case BTN_3:
                case BTN_4:
                case BTN_5:
                case BTN_6:
                case BTN_7:
                case BTN_8:
                case BTN_9:
                    glue::pass_event_queue.push((uint8_t)irrecv.decodedIRData.command);
                    break;
                default:
                    break;
                }
            }
            state = Re_Delay;
            break;
        case Re_Delay:
            state = Re_Run;
            irrecv.resume();
            break;
        }
        return state;
    }
} // namespace remote
#endif