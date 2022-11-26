#include "glue.h"
#include "lcd.h"

#include <Arduino.h>


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
        Component **components;
        uint8_t *previous;
        uint8_t pr_pos = 0;
        uint8_t current = UINT8_MAX - 1;

        Menu(Component **components, uint8_t *previous);
        uint8_t add(Component *p) {
            current = (current + 1) % UINT8_MAX;
            previous[pr_pos] = current;
            components[current] = p;
            return current;
        }
        inline Component* get_current() {
            return components[current];
        }
        void open(uint8_t to_open);
        void close(uint8_t offset);
        void tick();
    };

    extern Menu menu;
} // namespace display
#endif