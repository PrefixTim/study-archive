#include "glue.h"

namespace glue { // contains all the shared variables
    bool flag_armed = false;
    bool enable_input = true;
    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<LockReq> lock_req_queue(2);
    LimitedQueue<SetTimeStr> set_time_queue(2);
}