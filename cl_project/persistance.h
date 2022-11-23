#include "mtime.h"

#ifndef Persistance_h
#define Persistance_h
namespace persistance {
    struct Saved {
        time::TimeBcd turn_on_schedule[7];
        time::TimeBcd turn_off_schedule[7];
    };

    extern Saved global_state;

} // namespace persistance
#endif
