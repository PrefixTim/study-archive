#include "tmalk001_alarm.h"
#include "tmalk001_dsclock.h"
#include "tmalk001_glue.h"
#include "tmalk001_remote_codes.h"
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
            while (!glue::sensor_event_queue.is_empty()) {
                Serial.println(glue::sensor_event_queue.pop().type);
                if (glue::is_buzz_enabled(mtime::clock.t.time.week_day - 1))
                    glue::set_buzz_active(true);
                if (glue::is_led_enabled(mtime::clock.t.time.week_day - 1))
                    glue::set_led_active(true);
            }

            if (!glue::lock_req_queue.is_empty() && glue::lock_req_queue.pop().type == glue::LockReq::Unlock) {
                state = Al_Disarmed;
                glue::disarm();
                glue::set_buzz_active(false);
                glue::set_led_active(false);
            }
            break;
        case Al_Disarmed:
            while (!glue::sensor_event_queue.is_empty())
                glue::sensor_event_queue.pop();
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
            if (i == 0) {
                menu.close(1);
            }
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

    AlarmSetIOMenu::AlarmSetIOMenu() {
        i = 0;
        j = 0;
    }
    void AlarmSetIOMenu::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            i++;
            j <<= 1;
            break;
        case e.Inc:
            if (i == 0) {
                w +=e.val;
                w %= 7;
            } else {
                j &= 0xE;
            }
            break;
        case e.Dec:

            if (i == 0) {
                w-=e.val;
                if (w > 100) w = 6;
            } else {
                j |= 0x1;
            }
            break;
        }
        if (i == 5) {
            glue::schedule.io_flags[w] = (j >> 1);
            menu.close(1);
        }
    }
    void AlarmSetIOMenu::start() {
        lcd.clear();
        i = 0;
        j = 0;
        w = 0;
    }
    void AlarmSetIOMenu::update() {
        lcd.setCursor(0, 0);
        if (i == 0) {
            lcd.print("Select day?");
            lcd.setCursor(0, 1);
            lcd.print(mtime::TimeBcd::week_days[w + 1]);
        } else {
            if (i == 1) {
                lcd.print("Use led 4 out?");
            } else if (i == 2) {
                lcd.print("Use buz 4 out?");
            } else if (i == 3) {
                lcd.print("Use motion 4in?");
            } else if (i == 4) {
                lcd.print("Use light 4 in?");
            }
            lcd.setCursor(0, 1);
            lcd.print((j & 0x1) != 0 ? "<Yes>  No " : " Yes  <No>");
        }
    }
} // namespace display