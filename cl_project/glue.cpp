#include "glue.h"

namespace glue { // contains all the shared variables
    uint8_t enable_input = 1;
    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<TimeEvent> time_event_queue(5);
}