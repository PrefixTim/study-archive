/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 5  Exercise 2
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/ysAsYp74Yh8
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

unsigned char direction = -1;
unsigned int halt = 0;
int StepMotor_Tick(int state) {
    static unsigned char i = 0;
    if (!halt) {
        return state;
    }
    halt--;
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

enum JS_States {
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

enum Lock_States { Lock_Start,
                   Lock_Locked,
                   Lock_Failed,
                   Lock_Unlocking,
                   Lock_Open,
                   Lock_Locking,
                   Lock_Set };
int Lock_Tick(int state) {
    static int passwd[4] = {JS_L, JS_L, JS_U, JS_U};
    static int enter[4] = {0, 0, 0, 0};
    static int pr_input = JS_N;
    static unsigned char i = 0;
    int input = JS_read();
    switch (state) {
        case Lock_Start:
            state = Lock_Locked;
            break;
        case Lock_Locked:
            if (i == 4) {
                if (input != JS_N) break;
                for (i = 0; i < 4; i++) {
                    if (enter[i] != passwd[i]) {
                        i = 0;
                        leds = 0;
                        state = Lock_Failed;
                        break;
                    }
                }
                if (!i) break;
                i = 0;
                halt = 1800;
                direction = 1;
                state = Lock_Unlocking;
                break;
            }
            if (input == JS_B || input == JS_N || pr_input != JS_N) break;
            enter[i++] = input;
            break;
        case Lock_Failed:
            if (i > 3*5) {
                i = 0;
                leds = 0;
                state = Lock_Locked;
            }
            break;
        case Lock_Unlocking:
            if (!halt) state = Lock_Open;
            break;
        case Lock_Open:
            if (input == JS_B) {
                state = Lock_Set;
                break;
            }
            if (i == 4) {
                if (input != JS_N) break;
                for (i = 0; i < 4; i++) {
                    if (enter[i] != passwd[i]) {
                        i = 0;
                        break;
                    }
                }
                if (!i) break;
                i = 0;
                halt = 1800;
                direction = -1;
                state = Lock_Locking;
                break;
            }
            if (input == JS_N || pr_input != JS_N) break;
            enter[i++] = input;
            break;
        case Lock_Locking:
            if (!halt) state = Lock_Locked;
            break;
        case Lock_Set:
            if (i == 4) {
                if (input != JS_N) break;
                for (i = 0; i < 4; i++)
                    passwd[i] = enter[i];
                i = 0;
                state = Lock_Open;
                break;
            }
            if (input == JS_B || input == JS_N || pr_input != JS_N) break;
            enter[i++] = input;
            break;
    }

    pr_input = input;
    switch (state) {
        case Lock_Failed:
            if (i % 3 == 0) leds ^= 0xF;
            i++;
            break;
        case Lock_Unlocking:
        case Lock_Open:
        case Lock_Locking:
            leds = 0xf;
            break;
        case Lock_Locked:
        case Lock_Set:
            leds = ((pr_input == JS_U) << 0) | ((pr_input == JS_D) << 1) | ((pr_input == JS_L) << 2) | ((pr_input == JS_R) << 3);
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
    tasks[1] = {.state = Lock_Start, .period = 150, .lastRun = 0, .TickFct = &Lock_Tick};
    tasks[2] = {.state = 0, .period = 150, .lastRun = 0, .TickFct = &Led_Tick};
    Serial.begin(115200);
}

void loop() {
    for (int i = 0; i < NUM_TASK; i++) {
        if (millis() - tasks[i].lastRun >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].lastRun = millis();
        }
    }
}