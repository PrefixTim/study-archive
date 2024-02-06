#include "tmalk001_glue.h"

namespace glue { // contains all the shared variables
    bool flag_armed = false;
    bool enable_input = true;
    bool buzz_active = false;
    bool led_active = false;
    Schedule schedule = {
        .turn_on_schedule = {{127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}},
        .turn_off_schedule = {{127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}},
        .io_flags = {0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF}
    };

    LimitedQueue<SensorEvent> sensor_event_queue(3);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<LockReq> lock_req_queue(3);
    LimitedQueue<SetTimeStr> set_time_queue(2);
}