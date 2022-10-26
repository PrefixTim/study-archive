/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 4  Exercise 3
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/FOG9ksiotUk
 */
double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
int blp = 11;
int glp = 10;

#define NUM_TASK 4
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

static int phot = 0;
int Tick_Phot(int state) { //Single Task SM
    int tmp = analogRead(PHOT);
    phot = abs(tmp-phot) >= 20? tmp: phot;
    return state;
}

static int pot = 0;
int Tick_Pot(int state) { //Single Task SM
    pot = analogRead(POT);
    return state;
}

int Tick_Led(int state) {
    set_leds((1<<(map(phot, 100, 250, 0, 4)))-1);
    return state;
}

enum RGB_States {RGB_Start, RGB_On, RGB_Off};
int Tick_RGB(int state) {
    double tmp = map_double(pot, 0., 1023., -.3, 13.);
   switch (state) {
    case RGB_Start:
        state = tmp <= 0? RGB_Off: RGB_On;
        break;
    case RGB_Off:
        state = tmp <= 0? RGB_Off: RGB_On;
        set_rgb(0, 0, 0);
        break;
    case RGB_On:
        state = tmp <= 0? RGB_Off: RGB_On;
        set_rgb( 122*(tanh(tmp-2)+1.01), 122*(tanh(tmp-10)+1.01), 122*(tanh(tmp-6)+1.01));
        break;
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
    tasks[0] = { .state = 0, .period = 100, .lastRun = 0, .TickFct = &Tick_Pot };
    tasks[1] = { .state = 0, .period = 100, .lastRun = 0, .TickFct = &Tick_Phot };
    tasks[2] = { .state = 0, .period = 100, .lastRun = 0, .TickFct = &Tick_Led };
    tasks[3] = { .state = RGB_Start, .period = 100, .lastRun = 0, .TickFct = &Tick_RGB };
    Serial.begin(9600);
}

void loop() {
    for (int i = 0; i < NUM_TASK; i++) {
        if (millis() - tasks[i].lastRun >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].lastRun = millis();
        }
    }
}