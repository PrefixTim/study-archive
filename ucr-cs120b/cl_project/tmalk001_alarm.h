#include "tmalk001_menu.h"

#ifndef Alarm_h
#define Alarm_h
namespace alarm {
    enum Al_State{Al_Start, Al_Armed, Al_Arming, Al_Disarmed, Al_Disarming};
    int AlarmTickFct(int state);
} // namespace alarm
namespace display {
    struct AlarmToLock : public Component {
        uint8_t i;
        AlarmToLock();
        static uint8_t _new() {
            return menu.add(new AlarmToLock());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
    struct AlarmToUnLock : public Component {
        uint8_t i;
        AlarmToUnLock();
        static uint8_t _new() {
            return menu.add(new AlarmToUnLock());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
    struct AlarmSetPasswd : public Component {
        uint8_t i;
        AlarmSetPasswd();
        static uint8_t _new() {
            return menu.add(new AlarmSetPasswd());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };

        struct AlarmSetIOMenu : public Component {
        uint8_t i, j, w;
        AlarmSetIOMenu();
        static uint8_t _new() {
            return menu.add(new AlarmSetIOMenu());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif