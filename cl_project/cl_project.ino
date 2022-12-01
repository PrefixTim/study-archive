#include "task.h"
#include <LiquidCrystal.h>

void setup() {
    Serial.begin(115200);
    tasking::force_tick_tasks();
}

void loop() {
    tasking::tick_tasks();
}
