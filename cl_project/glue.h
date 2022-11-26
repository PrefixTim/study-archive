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

        void push(T v) {
            ptr[e++] = v;
            e %= size;
            if (e == b)
                b = (b + 1) % size;
        }

        T pop() {
            // if (is_empty())
            //     return;
            T tmp = ptr[b++];
            b %= size;
            return tmp;
        }

        uint8_t is_empty() {
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
            Press
        };
        EventType type;
        uint8_t val;
    };

    struct TimeEvent {
    };

    struct SensorEvent {
    };

    extern uint8_t enable_input;
    extern LimitedQueue<SensorEvent> sensor_event_queue;
    extern LimitedQueue<InputEvent> input_event_queue;
    extern LimitedQueue<TimeEvent> time_event_queue;
} // namespace glue
#endif
