#include "glue.h"

namespace glue { // contains all the shared variables
    bool flag_armed = false;
    bool enable_input = true;

    Schedule schedule = {
        .turn_on_schedule = {{127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}},
        .turn_off_schedule = {{127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}, {127, 127, 127, 127, 127, 127, 127}}
    };


    LimitedQueue<SensorEvent> sensor_event_queue(5);
    LimitedQueue<InputEvent> input_event_queue(10);
    LimitedQueue<LockReq> lock_req_queue(3);
    LimitedQueue<SetTimeStr> set_time_queue(2);
}