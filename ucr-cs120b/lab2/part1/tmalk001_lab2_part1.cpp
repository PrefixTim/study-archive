/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 2  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *        
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/zF5uoOL2Ruo
 */
#define X 6
#define Y 7

int leds[4] = {2, 3, 4, 5};
//int potentiometer
enum State {Start, OFF, ON} state;


void setup() {
    for(int i: leds) {
        pinMode(i, OUTPUT);
    }
    
    pinMode(X, INPUT);
    pinMode(Y, INPUT_PULLUP);

    state = Start;
    Serial.begin(9600);
}

void set_leds(short state){
    for (int i: leds) {
        digitalWrite(i, state);
    }
}

void Tick() {
    switch (state) {
        case Start:
            state = OFF;
            break;
        case OFF:
            if (!digitalRead(Y)) {
                state=ON;
                set_leds(HIGH);
            } else {
                state=OFF; 
            }      
            break;
        case ON:
            if (digitalRead(X)) {
                state=OFF;
                set_leds(LOW);
            } else {
                state=ON; 
            }      
            break;
        
    }

}


void loop() {
  // put your main code here, to run repeatedly:
    Tick();
}