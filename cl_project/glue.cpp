#include "glue.h"

namespace glue { // contains all the shared variables
    uint8_t passwd_dgt_enterd = 0;
    bool flag_armed = 0;
    bool enable_input = 1;
    bool clock_change = 0;
    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<uint8_t> pass_event_queue(4);
    LimitedQueue<TimeEvent> time_event_queue(5);
}