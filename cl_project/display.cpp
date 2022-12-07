#include <stdint.h>

#include "display.h"
#include "glue.h"
#include "alarm.h"
#include "list_menu.h"
#include "menu.h"
#include "menu_main.h"
#include "menu_set_alarm.h"
#include "menu_set_time.h"
namespace display {
    void start();
    LCD1602REG lcd(11, 12, 13);


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

    Component *tmp1[10] = {};
    uint8_t tmp2[10] = {};
    Menu menu(tmp1, tmp2);

    Entry a = {"Lock           ", AlarmToLock::_new()};
    Entry b = {"Set Time       ", SetTimeMenu::_new()};
    Entry c = {"Set Date       ", SetDateMenu::_new()};
    Entry d = {"Back           ", BackComp::_new()};
    Entry *e[4] = {&a, &b, &c, &d};

    void start() {
        lcd.begin();
        // Entry e[2] = { {"Se Alarm OnTime", AlarmOnSet::_new()}, {"Back           ", BackComp::_new()}}; //Fuck
        MenuMain::_new(ListMenu::_new(e, 4));
        menu.get_current()->start();
    }
} // namespace display