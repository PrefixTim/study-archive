#include "menu.h"

#ifndef AlarmMenu_h
#define AlarmMenu_h
namespace display {
    struct AlarmOnSet : public Component {
        int state = 0;
        static uint8_t _new() {
            return menu.add(new AlarmOnSet());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif