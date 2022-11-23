#include "glue.h"

namespace glue { // contains all the shared variables
    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<TimeEvent> time_event_queue(5);
}