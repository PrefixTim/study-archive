#include "menu.h"
namespace display {
    Menu::Menu(Component **_copmponents, uint8_t *_previous) : copmponents(_copmponents), previous(_previous) {}

    void Menu::open(uint8_t to_open) {
        previous[++pr_pos] = to_open;
        current = to_open;
        get_current()->start();
    }

    void Menu::close() {
        current = previous[--pr_pos];
        get_current()->start();
    }

    void Menu::tick() {
        while (!glue::input_event_queue.is_empty())
            get_current()->listen(glue::input_event_queue.pop());
        get_current()->update();
    }
    // display::Menu *p[2] = {new display::ListMenu(), new display::SetTurnOn()};

} // namespace display