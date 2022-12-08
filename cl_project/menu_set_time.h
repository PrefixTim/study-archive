#include "menu.h"

#ifndef SetTimeMenu_h
#define SetTimeMenu_h
namespace display {
    struct SetTimeMenu : public Component {
        uint8_t h = 0;
        uint8_t m = 0;
        uint8_t s = 0;
        uint8_t i = 0;
        static uint8_t _new() {
            return menu.add(new SetTimeMenu());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };

    struct SetDateMenu : public Component {
        uint8_t y = 0;
        uint8_t m = 0;
        uint8_t d = 0;
        uint8_t i = 0;
        static uint8_t _new() {
            return menu.add(new SetDateMenu());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };

    struct SetWeekDayMeny : public Component {
        uint8_t w = 0;
        uint8_t i = 0;
        static uint8_t _new() {
            return menu.add(new SetWeekDayMeny());
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif