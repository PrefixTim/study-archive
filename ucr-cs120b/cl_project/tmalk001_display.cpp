#include <stdint.h>

#include "tmalk001_alarm.h"
#include "tmalk001_alarm_sch.h"
#include "tmalk001_display.h"
#include "tmalk001_glue.h"
#include "tmalk001_list_menu.h"
#include "tmalk001_menu.h"
#include "tmalk001_menu_main.h"
#include "tmalk001_menu_set_alarm.h"
#include "tmalk001_menu_set_time.h"
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

    Component *tmp1[15] = {};
    uint8_t tmp2[10] = {};
    Menu menu(tmp1, tmp2);

    uint8_t back = BackComp::_new();
    Entry s1 = {"Set Time       ", SetTimeMenu::_new()};
    Entry s2 = {"Set Date       ", SetDateMenu::_new()};
    Entry s3 = {"Set Week Day   ", SetWeekDayMeny::_new()};
    Entry s4 = {"Set Pass       ", AlarmSetPasswd::_new()};
    Entry s5 = {"Set Schedule   ", AlarmSetMenu::_new()};
    Entry s6 = {"Set Trig&Outp  ", AlarmSetIOMenu::_new()};
    Entry sb = {"Back           ", back};
    Entry *set[] = {&s1, &s2, &s3, &s4, &s5, &s6, &sb};

    Entry m1 = {"Lock           ", AlarmToLock::_new()};
    Entry m2 = {"Settings       ", ListMenu::_new(set, 7)};
    Entry m3 = {"Back           ", back};

    Entry *m[] = {&m1, &m2, &m3};

    void start() {
        lcd.begin();
        MenuMain::_new(ListMenu::_new(m, 3), AlarmToUnLock::_new());
        // Entry e[2] = { {"Se Alarm OnTime", AlarmOnSet::_new()}, {"Back           ", BackComp::_new()}}; //Fuck
        menu.get_current()->start();
    }
} // namespace display