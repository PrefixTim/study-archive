#include <Arduino.h>
#include "tmalk001_task.h"

void setup() {
    Serial.begin(115200);
    tasking::force_tick_tasks();
}

void loop() {
    tasking::tick_tasks();
}
