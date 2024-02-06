 #ifndef Display_h
#define Display_h
namespace display {
    enum DP_State {
        DP_Start,
        DP_Run
    };
    int DisplayTickFct(int state);
} // namespace display
#endif