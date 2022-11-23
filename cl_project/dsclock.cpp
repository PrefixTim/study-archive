#include "dsclock.h"

namespace time {
    DS1307 clock;

    int ClockTick(int state){
        clock.readTime();
        return state;
    }
}