#include "glue.h"

namespace glue { // contains all the shared variables
    uint8_t passwd_dgt_enterd = 0;
    bool flag_armed = false;
    bool enable_input = true;
    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<uint8_t> pass_event_queue(4);
    LimitedQueue<LockReq> lock_req_queue(2);
    LimitedQueue<SetTimeStr> set_time_queue(2);
}