#include <Arduino.h>

#ifndef Output_H
#define Output_H
namespace output {
    enum Buzz_State { Buzz_Start,
                      Buzz_Run,
                      Buzz_Wait };
    int BuzzTickFct(int state);

    enum Led_State { Led_Start,
                     Led_Run,
                     Led_Wait };
    int LedTickFct(int state);
} // namespace output
#endif