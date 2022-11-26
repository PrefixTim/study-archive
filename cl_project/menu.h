#include "glue.h"
#include "lcd.h"

#ifndef Menu_h
#define Menu_h
namespace display {
    extern LCD1602REG lcd;

    struct Component {
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };

    struct Menu {
        Component **copmponents;
        uint8_t *previous;
        uint8_t pr_pos = 0;
        uint8_t current = 0;

        Menu(Component **copmponents, uint8_t *previous);
        inline Component* get_current() {
            return copmponents[current];
        }
        void open(uint8_t to_open);
        void close();
        void tick();
    };

    extern Menu menu;
} // namespace display
#endif