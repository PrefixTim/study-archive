/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 6  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: 
 */

typedef struct Task {
    int state;
    unsigned int period;
    unsigned long lastRun;
    int (*TickFct)(int);
} Task;

#define WaterLevel      A0
#define IceLevel        A1
#define WaterLevelLed   4
#define IceLevelLed     2
#define WaterPump       3
#define NitrogenPump    5
#define dispenser       6

#define NUM_TASK 1
Task tasks[NUM_TASK];

unsigned char pwms[3][4] = {
    {WaterPump, 30, 6,  30},
    {NitrogenPump, 50, 35, 50},
    {dispenser, 10, 1,  20}
};
int PWM_Tick(int state) {//single state Task
    static unsigned char j = 3;
    static unsigned char waite_time = 0;
    static unsigned char cycles = 0;
    static unsigned char whater_level = 0;
    static unsigned char ice_level = 0;
    static unsigned char i = 0;
    Serial.println(j);
    if(i % 5 == 0)whater_level = analogRead(WaterLevel) < 200;
    if (!(i++)) ice_level = analogRead(IceLevel) < 200;
    i %= 20;
    Serial.println(whater_level);
    Serial.println(ice_level);
    digitalWrite(WaterLevelLed, whater_level && !ice_level);
    digitalWrite(IceLevelLed, !whater_level && ice_level);
    if (whater_level || ice_level) {
        digitalWrite(pwms[j][0], LOW);
        return state;
    }
    if (waite_time == 0) {
        digitalWrite(pwms[j][0], LOW);
        j = (j + 1) % 4;
        waite_time = pwms[j][3];
        cycles = 0;
    }
    digitalWrite(pwms[j][0], cycles++<pwms[j][2]);
    waite_time -= (cycles % 10) == 0;
    cycles %= pwms[j][1];
    return state;
}


void setup() {
    for (char i = 2; i < 7; i++) pinMode(i, OUTPUT);
    tasks[0] = { .state = 0, .period = 10, .lastRun = 0, .TickFct = &PWM_Tick };
    Serial.begin(115200);
}

void loop() {
    Serial.println("loop");
    for (int i = 0; i < NUM_TASK; i++) {
        if (millis() - tasks[i].lastRun >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].lastRun = millis();
        }
    }
}
