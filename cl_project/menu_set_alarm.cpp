#include"menu_set_alarm.h"
namespace display {
        void AlarmOnSet::listen(glue::InputEvent e) {
            switch (e.type) {
            case e.Press:
                menu.close(1);
                break;
            case e.Inc:
                state += 1;
                break;
            case e.Dec:
                state -= 1;
                break;
            }
        }
        void AlarmOnSet::start() {
            lcd.setCursor(0, 0);
            lcd.print("Slct T to Arm Alarm");
            lcd.setCursor(0, 1);
            lcd.print("W:* H:**, M:** S:**");
        }

        void AlarmOnSet::update() {
            lcd.home();
            lcd.print(state);
        }
} // namespace display