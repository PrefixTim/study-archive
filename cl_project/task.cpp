#include "task.h"
#include "dsclock.h"
#include "rotaryenc.h"
#include "display.h"
#include "remote.h"
#include "alarm.h"
#include "alarm_sch.h"

namespace tasking {
    void Task::tick() {
        state = TickFct(state);
        lastRun = millis();
    }

    Task tasks[] = {
        {mtime::Clock_Start, 1000, 0, mtime::ClockTick},
        {rotary_encoder::RE_Start, 50, 0, rotary_encoder::RE_TickFct},
        {display::DP_Start, 400, 0, display::DisplayTickFct},
        {remote::Re_Start, 300, 0, remote::RemoteTickFct},
        {alarm::Al_Start, 200, 0, alarm::AlarmTickFct},
        {alarm::Al_Sch_Start, 1000, 0, alarm::AlarmSchTickFct}
    };

    void force_tick_tasks() {
        for (auto &task : tasks) task.tick();
    }

    void tick_tasks() {
        for (auto &task : tasks) if (abs(millis() - task.lastRun) >= task.period) task.tick();
    }
}  // namespace task
