#include "menu.h"
#ifndef ListMenu_h
#define ListMenu_h
namespace display {
    struct Entry {
        char text[16];
        uint8_t p;
        // Entry(char text[16], uint8_t p);
    };

    struct BackComp : public Component {
        static uint8_t _new() {
            return menu.add(new BackComp);
        }
        virtual void listen(glue::InputEvent e){};
        virtual void start() {
            // lcd.clear();
            menu.close(2);
        };
        virtual void update(){};
    };

    
    struct ListMenu : public Component {
        uint8_t i = 0;
        int8_t offset = 0;
        uint8_t size;
        Entry *list;

        ListMenu(Entry _list[], uint8_t _size);
        static uint8_t _new(Entry _list[], uint8_t _size) {
            return menu.add(new ListMenu(_list, _size));
        }
        virtual void listen(glue::InputEvent e);
        virtual void start();
        virtual void update();
    };
} // namespace display
#endif