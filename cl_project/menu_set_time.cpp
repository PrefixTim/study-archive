#include "menu_set_time.h"
#include "mtime.h"

namespace display {
    void SetTimeMenu::listen(glue::InputEvent e) {
        if (e.type == e.Press) {
            if (i < 3) {
                i++;
            } else {
                if (i == 3) {
                    glue::set_time_queue.push({s, m, h, 128, 128, 128});
                    menu.close(2);
                    return;
                }
                menu.close(1);
            }
        } else {
            if (i == 0) {
                h += e.type == e.Inc ? e.val : -e.val;
                if (h > 100) h = 0;
                h %= 24;
            } else if (i == 1) {
                m += e.type == e.Inc ? e.val : -e.val;
                if (m > 100) m = 0;
                m %= 60;
            } else if (i == 2) {
                s += e.type == e.Inc ? e.val : -e.val;
                if (s > 100) s = 0;
                s %= 60;
            } else {
                if (e.type == e.Inc) {
                    i = 4;
                } else {
                    i = 3;
                }
            }
        }
    }
    void SetTimeMenu::start() {
        h = 0;
        m = 0;
        s = 0;
        i = 0;
        lcd.setCursor(0, 0);
        lcd.print("Set Time");
        lcd.setCursor(0, 1);
        lcd.print("H:**, M:** S:**");
    }

    void SetTimeMenu::update() {
        if (i < 3) {
            lcd.setCursor(2, 1);
            lcd.print(h/10);
            lcd.print(h%10);
            lcd.setCursor(8, 1);
            lcd.print(m/10);
            lcd.print(m%10);
            lcd.setCursor(13, 1);
            lcd.print(s/10);
            lcd.print(s%10);
        } else {
            lcd.setCursor(0, 0);
            lcd.print("Change?        ");
            lcd.setCursor(0, 1);
            lcd.print(i == 3 ? "<Yes>  No      " : " Yes  <No>     ");
        }
    }
    void SetDateMenu::listen(glue::InputEvent e) {
        if (e.type == e.Press) {
            if (i < 3) {
                i++;
            } else {
                if (i == 3) {
                    glue::set_time_queue.push({128, 128, 128, d, m, y});
                    menu.close(2);
                    return;
                }
                menu.close(1);
            }
        } else {
            if (i == 0) {
                y += e.type == e.Inc ? e.val : -e.val;
                if (y > 150) y = 0;
                y %= 100;
            } else if (i == 1) {
                m += e.type == e.Inc ? e.val : -e.val;
                if (m > 100) m = 0;
                m %= 12;
            } else if (i == 2) {
                d += e.type == e.Inc ? e.val : -e.val;
                if (d > 100) d = 0;
                d %= 31;
            } else {
                if (e.type == e.Inc) {
                    i = 4;
                } else {
                    i = 3;
                }
            }
        }
    }
    void SetDateMenu::start() {
        y = 0;
        m = 0;
        d = 0;
        lcd.setCursor(0, 0);
        lcd.print("Set Time");
        lcd.setCursor(0, 1);
        lcd.print("Y:** M:** D:**");
    }

    void SetDateMenu::update() {
        if (i < 3) {
            lcd.setCursor(2, 1);
            lcd.print(y/10);
            lcd.print(y%10);
            lcd.setCursor(8, 1);
            lcd.print(m/10);
            lcd.print(m%10);
            lcd.setCursor(13, 1);
            lcd.print(d/10);
            lcd.print(d%10);
        } else {
            lcd.setCursor(0, 0);
            lcd.print("Change?        ");
            lcd.setCursor(0, 1);
            lcd.print(i == 3 ? "<Yes>  No      " : " Yes  <No>     ");
        }
    }
} // namespace display