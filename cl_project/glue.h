#include <stdint.h>
#ifndef Glue_h
#define Glue_h
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
    };
    struct SensorEvent {
    };

    extern bool flag_armed;
    extern bool enable_input;
    extern LimitedQueue<SensorEvent> sensor_event_queue;
    extern LimitedQueue<InputEvent> input_event_queue;
    extern LimitedQueue<SetTimeStr> set_time_queue;
    extern LimitedQueue<LockReq> lock_req_queue;

    inline bool is_armed() { return flag_armed; }
    inline void arm() { flag_armed = true; }
    inline void disarm() { flag_armed = false; }
} // namespace glue
#endif
