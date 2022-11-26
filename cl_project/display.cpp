#include <Arduino.h>
#include <stdint.h>

#include "display.h"
#include "glue.h"
#include "list_menu.h"
#include "menu.h"
#include "menu_set_alarm.h"


namespace display {
    void start();
    LCD1602REG lcd(11, 12, 13);

    Component *tmp1[10] = {};
    uint8_t tmp2[10] = {};
    Menu menu(tmp1, tmp2);

    int DisplayTickFct(int state) {
        switch (state) {
        case DP_Start:
            state = DP_Run;
            start();
            break;
        case DP_Run:
            menu.tick();
            break;
        }
        return state;
    }

    void start() {
        uint8_t i = 0;
        lcd.begin();
        Entry a = {"Se Alarm OnTime", AlarmOnSet::_new()};
        Entry b = {"Back           ", BackComp::_new()};
        Entry e[2] = {a, b};
        // Entry e[2] = { {"Se Alarm OnTime", AlarmOnSet::_new()}, {"Back           ", BackComp::_new()}}; //Fuck

        ListMenu::_new(e, 2);
        menu.get_current()->start();
    }
} // namespace display