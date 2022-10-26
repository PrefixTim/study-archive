/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 4  Exercise 2
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/8HUNEfHw4p8
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

#define NUM_TASK 2
Task tasks[NUM_TASK];

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

int Tick_Pot(int state) { //Single Task SM
    static int prev_pot = 0;
    int pot = analogRead(POT);
    if (abs(pot-prev_pot) >= 5) {
        tasks[0].period = map(pot, 0, 1023, 100, 500);
        prev_pot =  pot;
    }
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
        .period = 500,
        .lastRun = 0,
        .TickFct = &Tick_Led
    };
    tasks[1] = {
        .state = 0,
        .period = 100,
        .lastRun = 0,
        .TickFct = &Tick_Pot
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