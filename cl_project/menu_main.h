#include "menu.h"

#ifndef MenuMain_h
#define MenuMain_h
namespace display {
    struct MenuMain : public Component {
        uint8_t p;
        MenuMain(uint8_t p);
        static uint8_t _new(uint8_t p) {
            return menu.add(new MenuMain(p));
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif