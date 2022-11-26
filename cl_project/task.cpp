#include <Arduino.h>
#include "task.h"
#include "dsclock.h"
#include "rotaryenc.h"
#include "display.h"


namespace tasking {
    void Task::tick() {
        state = TickFct(state);
        lastRun = millis();
    }

    Task tasks[] = {
        {mtime::Clock_Start, 1000, UINT32_MAX, mtime::ClockTick},
        {rotary_encoder::RE_State::RE_Start, 50, UINT32_MAX, rotary_encoder::RE_TickFct},
        {display::DP_Start, 400, 0, display::DisplayTickFct},
    };

    void force_tick_tasks() {
        for (auto &task : tasks) task.tick();
    }

    void tick_tasks() {
        for (auto &task : tasks) if (abs(millis() - task.lastRun) >= task.period) task.tick();
    }
}  // namespace task
