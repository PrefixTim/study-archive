#include "glue.h"
#include <Arduino.h>
#include <IRremote.h>
#include "remote_codes.h"

#ifndef Remote_h
#define Remote_h
namespace remote {

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
                    glue::input_event_queue.push({glue::InputEvent::Num, (uint8_t)irrecv.decodedIRData.command});
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