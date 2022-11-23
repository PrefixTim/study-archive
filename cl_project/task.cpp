#include <Arduino.h>
#include "task.h"
#include "dsclock.h"
#include "rotaryenc.h"


namespace tasking {
    void Task::tick() {
        state = TickFct(state);
        lastRun = millis();
    }

    Task tasks[] = {
        {0, 1000, 0, mtime::ClockTick},
        {0, 200, 0, rotary_encoder::RE_TickFct},
        {0, 50, 0, rotary_encoder::RE_Btn_TickFct},
        // {0, 400, 0, display::DisplayTickFct},
    };

    void force_tick_tasks() {
        for (auto &task : tasks) task.tick();
    }

    void tick_tasks() {
        for (auto &task : tasks) if (abs(millis() - task.lastRun) >= task.period) task.tick();
    }
}  // namespace task
