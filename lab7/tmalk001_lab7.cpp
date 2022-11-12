/*         Your Name & E-mail: Timofey Malko & tmalk001@ucr.edu
 *         Discussion Section: 22
 *         Assignment: Lab 7  Exercise 1
 *         Exercise Description: [optional - include for your own benefit]
 *
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *
 *         Demo Link: https://youtu.be/kkf-zbnevJg
 */

#include <DHT.h>
#include <LiquidCrystal.h>

typedef struct Task {
    int state;
    uint16_t period;
    uint32_t lastRun;
    int (*TickFct)(int);
    void tick() {
        state = TickFct(state);
        lastRun = millis();
    }
} Task;
Task tasks[7];

uint8_t set_temperture = 75;
uint8_t set_humidity = 30;
uint8_t current_temperture = 75;
uint8_t current_humidity = 30;
uint8_t isOn = false;

typedef struct Pwm {
   public:
    enum Pwm_States { Pwm_Start,
                      Pwm_On,
                      Pwm_Off };
    uint8_t pin;
    uint8_t cycles_high;
    uint8_t cycles_total;

    Pwm(uint8_t pin, uint8_t cycles_high, uint8_t cycles_total) : pin(pin), cycles_high(cycles_high), cycles_total(cycles_total) {}

    int tick(int state, bool condition) {
        switch (state) {
            case Pwm_Start:
                pinMode(pin, OUTPUT);
            case Pwm_Off:
                state = condition && isOn ? Pwm_On : Pwm_Off;
                break;
            case Pwm_On:
                if (!condition || !isOn) {
                    i = 0;
                    digitalWrite(pin, LOW);
                    state = Pwm_Off;
                }
                break;
        }
        switch (state) {
            case Pwm_On:
                digitalWrite(pin, i < cycles_high ? HIGH : LOW);
                i = (i + 1) % cycles_total;
                break;
            case Pwm_Off:
                break;
        }
        return state;
    }

   private:
    uint8_t i = 0;
} Pwm;

typedef struct JoyStick {
   public:
    enum JS_READ { JS_B,
                   JS_N,
                   JS_U,
                   JS_D,
                   JS_L,
                   JS_R };

    JS_READ pr_state = JS_N;
    JS_READ state = JS_N;

    JoyStick(uint8_t btn_pin, uint8_t x_pin, uint8_t y_pin) : _btn_pin(btn_pin), _x_pin(x_pin), _y_pin(y_pin) {}

    void begin() {
        pinMode(_btn_pin, INPUT_PULLUP);
    }

    void read() {
        pr_state = state;
        state = _read();
    }

   private:
    inline JS_READ _read() {
        int16_t j_x = analogRead(_x_pin) - 1024 / 2;
        int16_t j_y = analogRead(_y_pin) - 1024 / 2;
        int16_t j_btn = !digitalRead(_btn_pin);
        if (j_btn) return JS_B;
        if (abs(j_x) > abs(j_y)) {
            if (abs(j_x) >= 200) return j_x < 0 ? JS_D : JS_U;
        } else {
            if (abs(j_y) >= 200) return j_y < 0 ? JS_L : JS_R;
        }
        return JS_N;
    }

    uint8_t _btn_pin;
    uint8_t _x_pin;
    uint8_t _y_pin;
} JoyStick;

enum Dht_States { Dht_Start,
                  Dht_On };
int Dht_Tick(int state) {
    static DHT dht(A0, DHT11);
    switch (state) {
        case Dht_Start:
            dht.begin();
            state = Dht_On;
            break;
        case Dht_On:
            current_temperture = dht.readTemperature(true);
            current_humidity = dht.readHumidity();
            break;
    }
    return state;
}

enum Js_States { Js_Start,
                 Js_Temp,
                 Js_Hum,
                 Js_Btn };
int Js_Tick(int state) {
    static JoyStick js(12, A1, A2);
    switch (state) {
        case Js_Start:
            js.begin();
            state = Js_Btn;
            break;
        case Js_Temp:
            js.read();
            if (js.state == JoyStick::JS_D) {
                state = Js_Hum;
            } else if (js.pr_state == JoyStick::JS_N && js.state == JoyStick::JS_B) {
                state = Js_Btn;
                isOn = false;
            }
            break;
        case Js_Hum:
            js.read();
            if (js.state == JoyStick::JS_U)
                state = Js_Temp;
            else if (js.pr_state == JoyStick::JS_N && js.state == JoyStick::JS_B) {
                state = Js_Btn;
                isOn = false;
            }
            break;
        case Js_Btn:
            js.read();
            if (js.pr_state == JoyStick::JS_N && js.state == JoyStick::JS_B) {
                state = Js_Temp;
                isOn = true;
            }
            break;
    }

    switch (state) {
        case Js_Temp:
            if (js.state == JoyStick::JS_L && set_temperture != 0)
                set_temperture--;
            else if (js.state == JoyStick::JS_R && set_temperture != 99)
                set_temperture++;
            break;
        case Js_Hum:
            if (js.state == JoyStick::JS_L && set_humidity != 0)
                set_humidity--;
            else if (js.state == JoyStick::JS_R && set_humidity != 99)
                set_humidity++;
            break;
        case Js_Btn:
            break;
    }
    return state;
}

enum Lcd_States { Lcd_Start,
                  Lcd_On,
                  Lcd_Off };
int Lcd_Tick(int state) {
    static LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
    switch (state) {
        case Lcd_Start:
            lcd.begin(16, 2);
            lcd.clear();
            state = Lcd_Off;
            break;
        case Lcd_Off:
            if (isOn) {
                state = Lcd_On;
                lcd.print("Temp:** STemp:**");
                lcd.setCursor(0, 1);
                lcd.print("Hum:** SHum:**");
            }
            break;
        case Lcd_On:
            if (!isOn) {
                state = Lcd_Off;
                lcd.clear();
            }
            break;
    }

    switch (state) {
        case Lcd_On:
            lcd.setCursor(5, 0);
            lcd.print(current_temperture / 10);
            lcd.print(current_temperture % 10);
            lcd.setCursor(14, 0);
            lcd.print(set_temperture / 10);
            lcd.print(set_temperture % 10);
            lcd.setCursor(4, 1);
            lcd.print(current_humidity / 10);
            lcd.print(current_humidity % 10);
            lcd.setCursor(12, 1);
            lcd.print(set_humidity / 10);
            lcd.print(set_humidity % 10);
            break;
        case Lcd_Off:
            break;
    }
    return state;
}

int Humidifier_Tick(int state) {
    static Pwm humidifier(8, 1, 5);
    return humidifier.tick(state, current_humidity < set_humidity);
}
int Dehumidifier_Tick(int state) {
    static Pwm dehumidifier(9, 1, 4);
    return dehumidifier.tick(state, current_humidity > set_humidity);
}
int Cooler_Tick(int state) {
    static Pwm cooler(10, 3, 5);
    return cooler.tick(state, current_temperture > set_temperture);
}
int Heater_Tick(int state) {
    static Pwm heater(11, 19, 20);
    return heater.tick(state, current_temperture < set_temperture);
}

void setup() {
    Serial.begin(500000);
    tasks[0] = {Js_Start, 300, 0, .TickFct = &Js_Tick};
    tasks[1] = {Dht_Start, 500, 0, .TickFct = &Dht_Tick};
    tasks[2] = {Lcd_Start, 300, 0, .TickFct = &Lcd_Tick};
    tasks[3] = {Pwm::Pwm_Start, 200 / 5, 0, .TickFct = &Humidifier_Tick};
    tasks[4] = {Pwm::Pwm_Start, 400 / 4, 0, .TickFct = &Dehumidifier_Tick};
    tasks[5] = {Pwm::Pwm_Start, 300 / 5, 0, .TickFct = &Cooler_Tick};
    tasks[6] = {Pwm::Pwm_Start, 100 / 20, 0, .TickFct = &Heater_Tick};
}

void loop() {
    for (auto &task : tasks)
        if (millis() - task.lastRun >= task.period) task.tick();
}
