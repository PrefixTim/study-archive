#include "alarm.h"
#include "glue.h"
#include "remote_codes.h"
#include <Arduino.h>

namespace alarm {
    int AlarmTickFct(int state) {
        switch (state) {
        case Al_Start:
            state = Al_Disarmed;
            glue::enable_input = 1;
            glue::disarm();
            break;
        case Al_Armed:
            if (!glue::lock_req_queue.is_empty() && glue::lock_req_queue.pop().type == glue::LockReq::Unlock) {
                state = Al_Disarmed;
                glue::disarm();
            }
            break;
        case Al_Disarmed:
            if (!glue::lock_req_queue.is_empty() && glue::lock_req_queue.pop().type == glue::LockReq::Lock) {
                state = Al_Armed;
                glue::arm();
            }
            break;
        default:
            break;
        }
        return state;
    }
} // namespace alarm

namespace display {
    uint8_t passwd[] = {BTN_1, BTN_2, BTN_3, BTN_4, BTN_5, BTN_6};
    AlarmToLock::AlarmToLock() { i = 0; }
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
        i = 0;
        lcd.print("U want 2 lock?");
        lcd.setCursor(0, 1);
        lcd.print("<Yes>  No ");
    }
    void AlarmToLock::update() {
        lcd.setCursor(0, 1);
        lcd.print(i == 0 ? "<Yes>  No " : " Yes  <No>");
    }
    uint8_t en_passwd[] = {0, 0, 0, 0, 0, 0};
    AlarmToUnLock::AlarmToUnLock() { i = 0; }
    void AlarmToUnLock::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            menu.close(1);
            break;
        case e.Num:
            en_passwd[i++] = e.val;
            if (i == 6)
                menu.close(1);
            break;
        case e.Dec:
            i--;
            break;
        }
        if (i == 6) {
            bool tmp = true;
            for (uint8_t j = 0; j < 6; j++)
                if (en_passwd[j] != passwd[j])
                    tmp = false;
            if (tmp) {
                glue::lock_req_queue.push({glue::LockReq::Unlock});
            }
        }
    }
    void AlarmToUnLock::start() {
        lcd.clear();
        i = 0;
    }
    void AlarmToUnLock::update() {
        lcd.setCursor(0, 0);
        lcd.print("Enter Pass");
        lcd.setCursor(0, 1);
        for (uint8_t j = i; j != 0; j--)
            lcd.print("*");
        lcd.print("    ");
    }
    AlarmSetPasswd::AlarmSetPasswd() { i = 0; }
    void AlarmSetPasswd::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            menu.close(1);
            break;
        case e.Num:
            en_passwd[i++] = e.val;
            if (i == 6)
                menu.close(1);
            break;
        case e.Dec:
            i--;
            break;
        }
        if (i == 6) {
            for (uint8_t j = 0; j < 6; j++)
                passwd[j] = en_passwd[j];
        }
    }
    void AlarmSetPasswd::start() {
        lcd.clear();
        i = 0;
    }
    void AlarmSetPasswd::update() {
        lcd.setCursor(0, 0);
        lcd.print("Enter New Pass");
        lcd.setCursor(0, 1);
        for (uint8_t j = i; j != 0; j--)
            lcd.print("*");
        lcd.print("    ");
    }
} // namespace display