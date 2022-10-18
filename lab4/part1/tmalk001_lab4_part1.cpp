/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 4  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/UBmi59ZxioQ
 */
typedef struct Task {
    int state;
    unsigned int period;
    unsigned long lastRun;
    int (*TickFct)(int);
} Task;

#define POT A0
#define PHOT A1
int leds[4] = {5, 4, 3, 2};
int rlp = 9;
int blp = 10;
int glp = 11;

#define NUM_TASK 1
Task tasks[NUM_TASK];

int Tick_Led(int state);

void set_leds(short led_state) {
    for (int i : leds) {
        digitalWrite(i, led_state & 0x1);
        led_state >>= 1;
    }
}

void set_rgb(unsigned short r, unsigned short g, unsigned short b) {
    analogWrite(rlp, r);
    analogWrite(glp, g);
    analogWrite(blp, b);
}

enum Led_States { Led_Start, Led_UP, Led_DOWN };

int Tick_Led(int state) {
    static unsigned short led_state;
    switch (state) {
        case Led_Start:
            state = Led_DOWN;
            led_state = 0x2;
            break;
        case Led_UP:
            led_state <<= 1;
            state = led_state >= 0x8 ? Led_DOWN : Led_UP;
            break;
        case Led_DOWN:
            led_state >>= 1;
            state = led_state <= 0x1 ? Led_UP : Led_DOWN;
            break;
    }
    set_leds(led_state);
    return state;
}

void setup() {
    pinMode(rlp, OUTPUT);
    pinMode(glp, OUTPUT);
    pinMode(blp, OUTPUT);
    for (int i : leds) {
        pinMode(i, OUTPUT);
    }
    tasks[0] = {
        .state = Led_Start,
        .period = 1000,
        .lastRun = 0,
        .TickFct = &Tick_Led
    };
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    for (int i = 0; i < NUM_TASK; i++) {
        if (millis() - tasks[i].lastRun >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].lastRun = millis();
        }
    }
}