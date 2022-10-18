/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 3  Exercise 3
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/UBmi59ZxioQ
 */
#define POT A0
#define PHOT A1
int leds[4] = {5, 4, 3, 2};
int rlp = 9;
int blp = 10;
int glp = 11;

void setup() {
    pinMode(rlp, OUTPUT);
    pinMode(glp, OUTPUT);
    pinMode(blp, OUTPUT);
    for (int i : leds) {
        pinMode(i, OUTPUT);
    }
    Serial.begin(9600);
}

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


enum States { INIT, Dark, Bright };
States state = INIT;

// This function takes in the state as an argument & returns the current state
void Tick() {
    int brightness = map(analogRead(PHOT), 0, 200, 0, 1);
    switch (state) {  // State transitions
        case INIT:
            // State Transition
            state = Dark;
            break;

        case Dark:
            state = brightness?Bright:Dark;
            break;

        case Bright:
            state = brightness?Bright:Dark;
            break;
    }

    switch (state) {  // State Action

        case INIT:
            // State Action
            set_leds(0x0);
            break;
        case Dark:
            // State Action
            set_rgb(122+122*sin(millis()/1000.), 122+122*sin(millis()/1000.+PI/3.), 122+122*sin(millis()/1000.+PI*2./3.));
            set_leds(0);
            break;
        case Bright:
            // State Action
            set_rgb(0, 0, 0);
            set_leds((1<<(map(analogRead(POT), 0, 1023, 0, 4)))-1);
            break;

    }
}

void loop() {
    /// Default arduino function

    Tick();
}
