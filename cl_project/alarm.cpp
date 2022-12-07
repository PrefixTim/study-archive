#include "alarm.h"
#include "glue.h"
#include <Arduino.h>
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
                while (!glue::pass_event_queue.is_empty()) {
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
            glue::disarm();
            glue::passwd_dgt_enterd = 0;
            break;
        case Al_Armed:
            if (pass) {
                state = Al_Disarmed;
                glue::disarm();
            }
            break;
        case Al_Disarmed:
            if (pass) {
                state = Al_Armed;
                glue::arm;
            }
            break;
        default:
            break;
        }
        return state;
    }
} // namespace alarm

namespace display {
    AlarmToLock::AlarmToLock() { i=0; }
    void AlarmToLock::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            if (i == 0) {
                glue::lock_req_queue.push({glue::LockReq::Lock});
                menu.close(2);
            } else {
                menu.close(1);
            }
            break;
        case e.Inc:
            i = 1;
            break;
        case e.Dec:
            i = 0;
            break;
        }
    }
    void AlarmToLock::start() {
        lcd.clear();
        i=0;
        lcd.print("U want 2 lock?");
        lcd.setCursor(0, 1);
        lcd.print("<Yes>  No ");

    }
    void AlarmToLock::update() {
        lcd.setCursor(0, 1);
        lcd.print(i == 0 ? "<Yes>  No " : " Yes  <No>");
    }

} // namespace display