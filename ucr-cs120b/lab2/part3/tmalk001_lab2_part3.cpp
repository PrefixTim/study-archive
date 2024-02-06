/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 2  Exercise 3
 *         Exercise Description: [optional - include for your own benefit]
 *        
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/_7Xw-cflj_8
 */

#define POT A0
int leds[4] = {2, 3, 4, 5};

void setup() {
    for(int i: leds) {
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

void Tick() {
    set_leds(map(analogRead(POT), 0, 1023, 0, 15));

}


void loop() {
    Tick(); //seq code task
}