#include "tmalk001_menu_main.h"
#include "tmalk001_dsclock.h"

namespace display {
    MenuMain::MenuMain(uint8_t p, uint8_t p_passwd) : p(p), p_passwd(p_passwd) {
    }
    void MenuMain::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            if (!glue::is_armed())
                menu.open(p);
            break;
        case e.Num:
            glue::input_event_queue.push({glue::InputEvent::Num, e.val});
            menu.open(p_passwd);
            break;
        case e.Inc:
        case e.Dec:
            break;
        }
    }
    void MenuMain::start() {
        lcd.clear();
    }

    void MenuMain::update() {
        lcd.setCursor(0, 0);
        lcd.print(glue::is_armed() ? "Locked   " : "Unlocked ");
        lcd.setCursor(0, 1);
        mtime::clock.t.time.printWeekDay(lcd);
        lcd.print(" ");
        mtime::clock.t.time.printHours(lcd);
        lcd.print(":");
        mtime::clock.t.time.printMinutes(lcd);
        lcd.print(":");
        mtime::clock.t.time.printSeconds(lcd);
    }

} // namespace display