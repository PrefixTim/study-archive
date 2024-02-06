#include <stdint.h>
#include "tmalk001_mtime.h"
#ifndef Persistance_h
#define Persistance_h
namespace persistance {
    struct Saved {
        mtime::TimeBcd turn_on_schedule[7];
        mtime::TimeBcd turn_off_schedule[7];
    };

    extern Saved global_state;

} // namespace persistance
#endif
