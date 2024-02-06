#include <stdint.h>

#ifndef Task_H
#define Task_H
namespace tasking {
    struct Task {
        int state;
        uint16_t period;
        uint32_t lastRun;
        int (*TickFct)(int);
        void tick();
    };

    extern Task tasks[];
    void force_tick_tasks();
    void tick_tasks();
}  // namespace task
#endif
