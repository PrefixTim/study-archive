/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 6  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/dltOzbdH0lM
 */


// A better single state Task
// typedef struct Task {
//     int state;
//     unsigned int period;
//     unsigned long lastRun;
//     int (*TickFct)(int);
// } Task;

// #define WaterLevel      A0
// #define IceLevel        A1
// #define WaterLevelLed   4
// #define IceLevelLed     2
// #define WaterPump       3
// #define NitrogenPump    5
// #define dispenser       6

// #define NUM_TASK 1
// Task tasks[NUM_TASK];

// unsigned char pwms[3][4] = {
//     {WaterPump, 30, 6,  30},
//     {NitrogenPump, 50, 35, 50},
//     {dispenser, 10, 1,  20}
// };
// int PWM_Tick(int state) {//single state Task
//     static unsigned char j = 3;
//     static unsigned char waite_time = 0;
//     static unsigned char cycles = 0;
//     static unsigned char whater_level = 0;
//     static unsigned char ice_level = 0;
//     static unsigned char i = 0;
//     if(i % 5 == 0)whater_level = analogRead(WaterLevel) < 200;
//     if (!(i++)) ice_level = analogRead(IceLevel) < 200;
//     i %= 20;
//     digitalWrite(WaterLevelLed, whater_level && !ice_level);
//     digitalWrite(IceLevelLed, !whater_level && ice_level);
//     if (whater_level || ice_level) {
//         digitalWrite(pwms[j][0], LOW);
//         return state;
//     }
//     if (waite_time == 0) {
//         digitalWrite(pwms[j][0], LOW);
//         j = (j + 1) % 4;
//         waite_time = pwms[j][3];
//         cycles = 0;
//     }
//     digitalWrite(pwms[j][0], cycles++<pwms[j][2]);
//     waite_time -= (cycles % 10) == 0;
//     cycles %= pwms[j][1];
//     return state;
// }

// void setup() {
//     for (char i = 2; i < 7; i++) pinMode(i, OUTPUT);
//     tasks[0] = { .state = 0, .period = 10, .lastRun = 0, .TickFct = &PWM_Tick };
// }

// void loop() {
//     Serial.println("loop");
//     for (int i = 0; i < NUM_TASK; i++) {
//         if (millis() - tasks[i].lastRun >= tasks[i].period) {
//             tasks[i].state = tasks[i].TickFct(tasks[i].state);
//             tasks[i].lastRun = millis();
//         }
//     }
// }

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

#define NUM_TASK 2
Task tasks[NUM_TASK];

unsigned char pwms[3][4] = {
    {WaterPump, 30, 6,  30},
    {NitrogenPump, 50, 35, 50},
    {dispenser, 10, 1,  20}
};

unsigned char ice_level = 1;
unsigned char whater_level = 1;
int PWM_Tick(int state) {//single state Task
    static unsigned char j = 3;
    static unsigned char cycles = 0;
    static unsigned char waite_time = 0;
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

#define MIN_light_level 250
int PHOT_Tick(int state) {
    static unsigned char i = 0;
    whater_level = analogRead(WaterLevel) < MIN_light_level;
    if (!(i++)) ice_level = analogRead(IceLevel) < MIN_light_level;
    i %= 4;
    digitalWrite(WaterLevelLed, whater_level && !ice_level);
    digitalWrite(IceLevelLed, !whater_level && ice_level);
}

void setup() {
    for (char i = 2; i < 7; i++) pinMode(i, OUTPUT);
    tasks[0] = { .state = 0, .period = 10, .lastRun = 0, .TickFct = &PWM_Tick };
    tasks[1] = { .state = 0, .period = 50, .lastRun = 0, .TickFct = &PHOT_Tick };
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