#include "display.h"
namespace display {
    LCD1602REG lcd(11, 12, 13);

    int DisplayTickFct(int state) {
        switch (state) {
        case DP_Start:
            lcd.begin();
            state = DP_Run;
            break;
        case DP_Run:
            lcd.print("Hello");
            break;
        }
        return state;
    }

} // namespace display