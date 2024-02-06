#include "tmalk001_sensors.h"
#include "tmalk001_glue.h"
#include "tmalk001_dsclock.h"


namespace sensors {
    Usonic::Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void Usonic::begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

    int PhotTickFct(int state) {
        static uint8_t pr_v = 0;
        switch (state) {
        case Phot_Start:
            state = Phot_Wait;
            break;
        case Phot_Run:
            if (glue::is_phot_enabled(mtime::clock.t.time.week_day - 1)) {
                if( map(analogRead(A0), 0, 256, 0, 1)) {
                    if (pr_v) {
                        glue::sensor_event_queue.push({glue::SensorEvent::Photoresistor});
                        state = Phot_Wait;
                    }
                    pr_v = 1;
                }
            } else {
                state = Phot_Wait;
            }
            break;
        case Phot_Wait: 
            if ((!pr_v) && glue::is_phot_enabled(mtime::clock.t.time.week_day - 1)) {
                state = Phot_Run;
            }
            pr_v = glue::is_phot_enabled(mtime::clock.t.time.week_day - 1);
            break;
        }
        return state;
    }
    
    int MotionTickFct(int state) {
        static uint8_t pr_v = 0;
        switch (state) {
        case Motion_Start:
            pinMode(5, INPUT);
            state = Motion_Wait;
            break;
        case Motion_Run:
            if (glue::is_motion_enabled(mtime::clock.t.time.week_day - 1)) {
                if( digitalRead(5)) {
                    if (pr_v) {
                        glue::sensor_event_queue.push({glue::SensorEvent::MotionSensor});
                        state = Motion_Wait;
                    }
                    pr_v = 1;
                }
            } else {
                state = Motion_Wait;
            }
            break;
        case Motion_Wait: 
            if ((!pr_v) && glue::is_motion_enabled(mtime::clock.t.time.week_day - 1)) {
                state = Motion_Run;
            }
            pr_v = glue::is_motion_enabled(mtime::clock.t.time.week_day - 1);
            break;
        }
        return state;
    }
} // namespace sensors
