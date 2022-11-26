#include "menu.h"
#ifndef ListMenu_h
#define ListMenu_h
namespace display {
    struct Entry {
        char text[17];
        uint8_t p;
    };
    
    struct ListMenu : public Component {
        uint8_t i = 0;
        int8_t offset = -1;
        uint8_t size;
        Entry *list;

        ListMenu(Entry *_list, uint8_t _size);
        virtual void listen(glue::InputEvent e);
        virtual void printTo();
        virtual void update();
    };
} // namespace display
#endif