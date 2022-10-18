/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 2  Exercise 2
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code from this class, is my own original work.
 *
 *         Demo Link: https://youtu.be/zRTT94CAMTA
 */

typedef struct Task {
    int state;
    unsigned int period;
    unsigned int elapsedTime;
    int (*TickFct)(int);
} Task;

#define DELAY 50
#define X 6
#define Y 7
int leds[4] = {2, 3, 4, 5};
#define NUM_TASK 2
Task tasks[NUM_TASK];

void set_leds(short led_state) {
    for (int i : leds) {
        digitalWrite(i, led_state & 0x1);
        led_state >>= 1;
    }
}

enum XY_Stated { XY_Start, XY_S1, XY_S2, XY_S3 };

int Tick_XY(int state) {
    static char X_past;
    static char Y_past;
    char X_now = !digitalRead(X);
    char Y_now = digitalRead(Y);
    switch (state) {
        case XY_Start:
            state = XY_S2;
            X_past = X_now;
            Y_past = Y_now;
            break;
        case XY_S1:
            if (!X_now & X_past) {
                state = XY_S2;
                tasks[1].period = 750;
            } else {
                state = XY_S1;
            }
            break;
        case XY_S2:
            if (!X_now & X_past) {
                state = XY_S3;
                tasks[1].period = 1250;
            } else if (!Y_now & Y_past) {
                state = XY_S1;
                tasks[1].period = 250;
            } else {
                state = XY_S2;
            }
            break;
        case XY_S3:
            if (!Y_now & Y_past) {
                state = XY_S2;
                tasks[1].period = 750;
            } else {
                state = XY_S3;
            }
            break;
    }
    X_past = X_now;
    Y_past = Y_now;
    return state;
}

enum Led_States { Led_Start, Led_UP, Led_DOWN };

int Tick_Led(int state) {
    static short led_state;
    switch (state) {
        case Led_Start:
            state = Led_DOWN;
            led_state = 0;
            break;
        case Led_UP:
            led_state <<= 1;
            led_state++;
            state = led_state >= 0xF ? Led_DOWN : Led_UP;
            break;
        case Led_DOWN:
            led_state >>= 1;
            state = led_state <= 0x0 ? Led_UP : Led_DOWN;
            break;
    }
    set_leds(led_state);
    return state;
}

void setup() {
    for (int i : leds) {
        pinMode(i, OUTPUT);
    }

    pinMode(X, INPUT);
    pinMode(Y, INPUT_PULLUP);

    tasks[0].state = XY_Start;
    tasks[0].period = 50;
    tasks[0].elapsedTime = tasks[0].period;
    tasks[0].TickFct = &Tick_XY;

    tasks[1].state = Led_Start;
    tasks[1].period = 750;
    tasks[1].elapsedTime = tasks[1].period;
    tasks[1].TickFct = &Tick_Led;

    Serial.begin(19200);
}

void loop() {
    // put your main code here, to run repeatedly:
    for (int i = 0; i < NUM_TASK; i++) {
        if (tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += DELAY;
    }
    delay(DELAY);
}