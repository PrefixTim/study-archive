#include "menu.h"

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
} // namespace display
#endif