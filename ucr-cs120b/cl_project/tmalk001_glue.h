#include <stdint.h>
#ifndef Glue_h
#define Glue_h
#include "tmalk001_mtime.h"

namespace glue { // contains all the shared variables
    template <typename T>
    class LimitedQueue {
    public:
        LimitedQueue(uint8_t size) : b(0), e(0), size(size) {
            ptr = new T[size];
        }

        inline void push(T v) {
            ptr[e++] = v;
            e %= size;
            if (e == b)
                b = (b + 1) % size;
        }

        inline T pop() {
            // if (is_empty())
            //     return;
            T tmp = ptr[b++];
            b %= size;
            return tmp;
        }

        inline uint8_t is_empty() {
            return b == e;
        }

    private:
        uint8_t b, e;
        uint8_t size;
        T *ptr;
    };

    struct InputEvent {
        enum EventType {
            Inc,
            Dec,
            Press,
            Trig,
            Num
        };
        EventType type;
        uint8_t val;
    };

    struct LockReq {
        enum ReqType {
            Lock,
            Unlock
        };
        ReqType type;
    };

    struct SetTimeStr {
        uint8_t s;
        uint8_t m;
        uint8_t h;
        uint8_t d;
        uint8_t n;
        uint8_t y;
        uint8_t w;
    };
    struct SensorEvent {
        enum SensorType {
            Photoresistor,
            MotionSensor,
        };
        SensorType type;
    };

    struct Schedule {
        mtime::TimeBcd turn_on_schedule[7];
        mtime::TimeBcd turn_off_schedule[7];
        uint8_t io_flags[7];
    };

    extern bool flag_armed;
    extern bool enable_input;
    extern Schedule schedule;
    extern LimitedQueue<SensorEvent> sensor_event_queue;
    extern LimitedQueue<InputEvent> input_event_queue;
    extern LimitedQueue<SetTimeStr> set_time_queue;
    extern LimitedQueue<LockReq> lock_req_queue;

    inline bool is_armed() { return flag_armed; }
    inline void arm() { flag_armed = true; }
    inline void disarm() { flag_armed = false; }


    extern bool buzz_active;
    extern bool led_active;
    inline bool is_buzz_active() { return flag_armed && buzz_active; }
    inline bool is_led_active() { return flag_armed && led_active; }
    inline void set_buzz_active(uint8_t val) { buzz_active = val; }
    inline void set_led_active(uint8_t val) { led_active = val; }

    inline bool is_phot_enabled(uint8_t w) { return flag_armed && (schedule.io_flags[w] & 0x1); }
    inline bool is_motion_enabled(uint8_t w) { return flag_armed && (schedule.io_flags[w] & 0x2); }
    inline bool is_buzz_enabled(uint8_t w) { return flag_armed && (schedule.io_flags[w] & 0x4); }
    inline bool is_led_enabled(uint8_t w) { return flag_armed && (schedule.io_flags[w] & 0x8); }

    inline void set_phot_enabled(uint8_t w, uint8_t val) { schedule.io_flags[w] ^= (-val ^ schedule.io_flags[w]) & (0x1 << 0); }
    inline void set_motion_enabled(uint8_t w, uint8_t val) { schedule.io_flags[w] ^= (-val ^ schedule.io_flags[w]) & (0x1 << 1);  }
    inline void set_buzz_enabled(uint8_t w, uint8_t val) { schedule.io_flags[w] ^= (-val ^ schedule.io_flags[w]) & (0x1 << 2); }
    inline void set_led_enabled(uint8_t w, uint8_t val) { schedule.io_flags[w] ^= (-val ^ schedule.io_flags[w]) & (0x1 << 3);  }

} // namespace glue
#endif
