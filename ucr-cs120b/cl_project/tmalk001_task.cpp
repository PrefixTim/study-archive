#include "tmalk001_task.h"
#include "tmalk001_dsclock.h"
#include "tmalk001_rotaryenc.h"
#include "tmalk001_display.h"
#include "tmalk001_remote.h"
#include "tmalk001_alarm.h"
#include "tmalk001_alarm_sch.h"
#include "tmalk001_sensors.h"
#include "tmalk001_output.h"
#include <Arduino.h>
#include <stdint.h>


namespace tasking {
    void Task::tick() {
        state = TickFct(state);
        lastRun = millis();
    }

    Task tasks[] = {
        {mtime::Clock_Start, 1000, 0, mtime::ClockTick},
        {rotary_encoder::RE_Start, 50, 0, rotary_encoder::RE_TickFct},
        {display::DP_Start, 400, 399, display::DisplayTickFct},
        {remote::Re_Start, 300, 0, remote::RemoteTickFct},
        {alarm::Al_Start, 200, 0, alarm::AlarmTickFct},
        {alarm::Al_Sch_Start, 1000, 0, alarm::AlarmSchTickFct},
        {sensors::Phot_Start, 1000, 0, sensors::PhotTickFct},
        {sensors::Motion_Start, 1000, 0, sensors::MotionTickFct},
        {output::Buzz_Start, 3, 0, output::BuzzTickFct},
        {output::Led_Start, 500, 0, output::LedTickFct}

    };

    void force_tick_tasks() {
        for (auto &task : tasks) task.tick();
    }

    void tick_tasks() {
        for (auto &task : tasks) if (abs(millis() - task.lastRun) >= task.period) task.tick();
    }
}  // namespace task
