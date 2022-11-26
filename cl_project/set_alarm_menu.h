#include "menu.h"

#ifndef AlarmMenu_h
#define AlarmMenu_h
namespace display {
    struct SetTurnOn : public Menu {
        int state = 0;
        virtual void listen(glue::InputEvent e) {
            switch (e.type) {
            case e.Press:
                menu.close();
                break;
            case e.Inc:
                break;
            case e.Dec:
                break;
            }
        }
        virtual void printTo(LCD1602REG &p) {
            p.setCursor(0, 0);
            p.print("Slct T to Arm Alarm");
            p.setCursor(0, 1);
            p.print("W:* H:**, M:** S:**");
        }
        virtual void update(LCD1602REG &p) {
            lcd.home();
            lcd.print("Alarm set");
            // time.printHours(lcd);
            // lcd.print(":");
            // time.printMinutes(lcd);
            // lcd.print(":");
            // time.printSeconds(lcd);
            // lcd.print("  ");
            // time.printWeekDay(lcd);
            // lcd.setCursor(0, 1);
            // time.printYear(lcd);
            // lcd.print("/");
            // time.printMonth(lcd);
            // lcd.print("/");
            // time.printDay(lcd);
        }
    };
} // namespace display
#endif