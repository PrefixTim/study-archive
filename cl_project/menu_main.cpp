#include "menu_main.h"
#include "dsclock.h"

namespace display {
    MenuMain::MenuMain(uint8_t p) : p(p) {}
    void MenuMain::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            if (!glue::is_armed())
                menu.open(p);
            break;
        case e.Inc:
            break;
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
        if (glue::passwd_dgt_enterd == 0) {
            mtime::clock.t.time.printHours(lcd);
            lcd.print(":");
            mtime::clock.t.time.printMinutes(lcd);
            lcd.print(":");
            mtime::clock.t.time.printSeconds(lcd);
        } else {
            for (uint8_t i = glue::passwd_dgt_enterd; i != 0; i--) {
                lcd.print("*");
            }
            lcd.print("        ");
        }
    }

} // namespace display