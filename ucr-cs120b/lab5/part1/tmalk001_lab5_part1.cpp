/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 5  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/Zckm3CUtU68
 */

typedef struct Task {
    int state;
    unsigned int period;
    unsigned long lastRun;
    int (*TickFct)(int);
} Task;

#define NUM_TASK 3
Task tasks[NUM_TASK];

#define IN1 13
#define IN2 12
#define IN3 11
#define IN4 10

#define LED_UP 3
#define LED_DOWN 4
#define LED_LEFT 2
#define LED_RIGHT 5

#define JS_BTN 7
#define JS_X A0
#define JS_Y A1

int sig[4] = {IN1, IN2, IN3, IN4};
int steps[8][4]{
    // codes for the motor to turn properly
    {0, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 1},
};

unsigned char speed = 1;  // 0 to 3
unsigned char direction = -1;
unsigned char halt = 0;

int StepMotor_Tick(int state) {
    static unsigned char i = 0;
    static unsigned char wait = 0;
    if (halt) {
        return state;
    }
    if (wait--) return state;
    wait = 3 - speed;
    for (unsigned char c = 0; c < 4; c++) digitalWrite(sig[c], steps[i][c] == 1 ? HIGH : LOW);
    i = (8 + i + direction) % 8;
    return state;
}

unsigned char leds = 0x0;
int Led_Tick(int state) {
    digitalWrite(LED_UP, (leds >> 0) & 0x1 ? HIGH : LOW);
    digitalWrite(LED_DOWN, (leds >> 1) & 0x1 ? HIGH : LOW);
    digitalWrite(LED_LEFT, (leds >> 2) & 0x1 ? HIGH : LOW);
    digitalWrite(LED_RIGHT, (leds >> 3) & 0x1 ? HIGH : LOW);
    return state;
}

enum JS_SM_States {
    JS_Start,
    JS_B,
    JS_N,
    JS_U,
    JS_D,
    JS_L,
    JS_R
};

int JS_read() {
    int j_x = analogRead(JS_X) - 1024 / 2;
    int j_y = analogRead(JS_Y) - 1024 / 2;
    int j_btn = !digitalRead(JS_BTN);
    if (j_btn) {
        return JS_B;
    }
    if (abs(j_x) > abs(j_y)) {
        if (abs(j_x) >= 200) {
            return j_x < 0 ? JS_D : JS_U;
        }
    } else {
        if (abs(j_y) >= 200) {
            return j_y < 0 ? JS_L : JS_R;
        }
    }
    return JS_N;
}

int JS_Tick(int state) {
    switch (state) {
        case JS_Start:
            state = JS_R;
            break;
        case JS_B:
            state = JS_read() == JS_B ? JS_B : JS_N;
            break;
        case JS_U:
            state = JS_read() == JS_U ? JS_U : JS_N;
            break;
        case JS_D:
            state = JS_read() == JS_D ? JS_D : JS_N;
            break;
        case JS_L:
        case JS_R:
        case JS_N:
            state = JS_read();
            if (state == JS_U) {
                speed++;
                if (speed > 3) speed = 3;
            } else if (state == JS_D) {
                if (speed < 1) speed = 1;
                speed--;
            }
            break;
    }
    switch (state) {
        case JS_B:
            leds = 0xF;
            halt = 1;
            break;
        case JS_N:
            leds = direction == 1 ? 0x4 : 0x8;
            halt = 0;
            break;
        case JS_U:
            leds |= 0x1;
            break;
        case JS_D:
            leds |= 0x2;
            break;
        case JS_L:
            leds = 0x4;
            direction = 1;
            break;
        case JS_R:
            leds = 0x8;
            direction = -1;
            break;
    }
    return state;
}

void setup() {
    for (int i : sig) {
        pinMode(i, OUTPUT);
    }
    pinMode(LED_UP, OUTPUT);
    pinMode(LED_DOWN, OUTPUT);
    pinMode(LED_LEFT, OUTPUT);
    pinMode(LED_RIGHT, OUTPUT);
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