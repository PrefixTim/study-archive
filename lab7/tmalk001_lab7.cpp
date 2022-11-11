/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 7  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: 
 */
typedef struct Task { 
    int state; 
    uint16_t period; 
    uint32_t lastRun; 
    int (*TickFct)(int); 
} Task;

typedef struct Pwm_Task: public Task { 
    uint8_t _pin; 
    uint8_t _duty;
    uint8_t _enable; 
    
    PWM_Task(uint16_t period, uint32_t lastRun, uint8_t pin, uint8_t duty, uint8_t enable): Task(0, period, lastRun, &PwmTickFct), _pin(pin), _duty(duty), _enable(enable) {} 
    
    int PwmTickFct(int state) {return state;}
} Pwm_Task;

#define NUM_TASK 5
Task tasks[NUM_TASK];

typedef struct JoyStick {
public:
    enum JS_READ { JS_B, JS_N, JS_U, JS_D, JS_L, JS_R };
    JoyStick(uint8_t btn_pin, uint8_t x_pin, uint8_t y_pin){
        _btn_pin = btn_pin;
        _x_pin = x_pin;
        _y_pin = y_pin;
    }
    void begin() {
        pinMode(_btn_pin, INPUT_PULLUP);
    }
    JS_READ JS_read() {
        int j_x = analogRead(JS_X) - 1024 / 2;
        int j_y = analogRead(JS_Y) - 1024 / 2;
        int j_btn = !digitalRead(JS_BTN);
        if (j_btn) return JS_B;
        if (abs(j_x) > abs(j_y)) {
            if (abs(j_x) >= 200) return j_x < 0 ? JS_D : JS_U;
        } else {
            if (abs(j_y) >= 200) return j_y < 0 ? JS_L : JS_R;
        }
        return JS_N;
    }
private:
    uint8_t _btn_pin;
    uint8_t _x_pin;
    uint8_t _y_pin;
} JoyStick;

JoyStick js(7, A0, A1);

int JS_Tick(int state) {
  
    return state;
}

void setup() {
    pinMode(JS_BTN, INPUT_PULLUP);
    tasks[0] = {.state = 0, .period = 1, .lastRun = 0, .TickFct = &StepMotor_Tick};
    tasks[1] = {.state = JS_Start, .period = 100, .lastRun = 0, .TickFct = &JS_Tick};
    tasks[2] = {.state = 0, .period = 100, .lastRun = 0, .TickFct = &Led_Tick};
}

void loop() {
    for (int i = 0; i < NUM_TASK; i++) {
        if (millis() - tasks[i].lastRun >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].lastRun = millis();
        }
    }
}
