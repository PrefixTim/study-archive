#include "mtime.h"
#include "menu.h"

#ifndef Alarm_Sch_h
#define Alarm_Sch_h
namespace alarm { 
    enum Al_Sch_State{Al_Sch_Start, Al_Sch_Run, Al_Sch_Wait};
    int AlarmSchTickFct(int state);
} // namespace alarm
namespace display {

    struct AlarmSetMenu : public Component {
        uint8_t i;
        uint8_t w;
        uint8_t h;
        uint8_t m;
        AlarmSetMenu(){};
        static uint8_t _new() {
            return menu.add(new AlarmSetMenu());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
    
} // namespace display
#endif
