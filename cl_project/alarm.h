
#ifndef Alarm_h
#define Alarm_h
namespace alarm {
    enum Al_State{Al_Start, Al_Armed, Al_Arming, Al_Disarmed, Al_Disarming};
    int AlarmTickFct(int state);
} // namespace alarm
#endif