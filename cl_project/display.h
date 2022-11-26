#include "glue.h"
#include"menu.h"
#include"list_menu.h"
#include"set_alarm_menu.h"

#ifndef Display_h
#define Display_h
namespace display {
    enum DP_State {
        DP_Start,
        DP_Run
    };
    int DisplayTickFct(int state);
} // namespace display
#endif