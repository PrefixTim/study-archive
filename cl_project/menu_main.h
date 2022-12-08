#include "menu.h"

#ifndef MenuMain_h
#define MenuMain_h
namespace display {
    struct MenuMain : public Component {
        uint8_t p;
        uint8_t p_passwd;
        MenuMain(uint8_t p, uint8_t p_passwd);
        static uint8_t _new(uint8_t p, uint8_t p_passwd) {
            return menu.add(new MenuMain(p, p_passwd));
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif