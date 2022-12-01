#include "alarm.h"
#include "glue.h"
#include<Arduino.h>
namespace alarm {
    uint8_t passwd[] = {0xC, 0x18, 0x5E, 0x8};
    int AlarmTickFct(int state) {
        bool pass = 0;
        while (!glue::pass_event_queue.is_empty()) {
            auto d = glue::pass_event_queue.pop();
            if (d == passwd[glue::passwd_dgt_enterd]) {
                if (glue::passwd_dgt_enterd == 3) {
                    pass = 1;
                    while (!glue::pass_event_queue.is_empty())
                        glue::pass_event_queue.pop();
                }
            } else {
                while (!glue::pass_event_queue.is_empty()){
                    glue::passwd_dgt_enterd = (glue::passwd_dgt_enterd + 1) % 4;
                    glue::pass_event_queue.pop();
                }
            }
            glue::passwd_dgt_enterd = (glue::passwd_dgt_enterd + 1) % 4;
        }

        switch (state) {
        case Al_Start:
            state = Al_Disarmed;
            glue::enable_input = 1;
            glue::flag_armed = 0;
            glue::passwd_dgt_enterd = 0;
            break;
        case Al_Armed:
            if (pass) {
                state = Al_Disarmed;
                glue::flag_armed = false;
            }
            break;
        case Al_Disarmed:
            if (pass) {
                state = Al_Armed;
                glue::flag_armed = true;
            }
            break;
        default:
            break;
        }
        return state;
    }
} // namespace alarm