#include "tmalk001_list_menu.h"
#include <Arduino.h>
namespace display {
    ListMenu::ListMenu(Entry *_list[], uint8_t _size) : list(_list), size(_size) {}
    void ListMenu::listen(glue::InputEvent e) {
        switch (e.type) {
        case e.Press:
            menu.open(list[i]->p);
            break;
        case e.Inc:
            if (i != size - 1)
                i += 1;
            break;
        case e.Dec:
            if (i != 0)
                i -= 1;
            break;
        }
        if (i == size - 1)
            offset = -1;
        else if (i == 0)
            offset = 0;
    }

    void ListMenu::start() {
        // lcd.clear();
    }

    void ListMenu::update() {
        lcd.setCursor(0, 0);
        lcd.print(offset ? " " : "#");
        lcd.print(list[i + offset]->text);
        lcd.setCursor(0, 1);
        lcd.print(offset ? "#" : " ");
        lcd.print(list[i + offset + 1]->text);
    }
} // namespace display