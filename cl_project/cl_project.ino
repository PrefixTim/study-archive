#include <Print.h>
#include <Wire.h>
#include "glue.h"



namespace sensors {
class Usonic {
   public:
    Usonic(uint8_t echo_pin, uint8_t trig_pin) : echo_pin(echo_pin), trig_pin(trig_pin) {}

    void begin() {
        pinMode(trig_pin, OUTPUT);
        pinMode(echo_pin, INPUT);
    }

    inline float readDistance() {
        digitalWrite(trig_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(trig_pin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig_pin, LOW);
        return pulseIn(echo_pin, HIGH) * 17 / 1000;
    }

   private:
    uint8_t echo_pin;
    uint8_t trig_pin;
};

}  // namespace sensors
namespace display {
class LCD1602REG : public Print {
   public:
    using Print::write;
    LCD1602REG(uint8_t ser, uint8_t rclk, uint8_t srclk) : _ser(ser), _rclk(rclk), _srclk(srclk) {}
    void begin() {
        pinMode(_ser, OUTPUT);
        pinMode(_rclk, OUTPUT);
        pinMode(_srclk, OUTPUT);
        while (millis() < 20)
            ;           // must be at least 15ms since power on
        command(0x38);  // Functoin Set: DL=8, N=2, F=5x8
        command(0x0C);  // Display ON?OFF: D=1, C=0, B=0
        command(0x06);  // Entry mode
        command(0x01);  // Clear Display
    }

    size_t println(void) {
        setCursor(0, 1);
    }

    inline void setCursor(uint8_t col, uint8_t row) {
        command(0x80 | (col + row * 0x40));
    }

    inline void clear() {
        command(0x1);
        delayMicroseconds(1520);  // delay 1.52 ms between call for CLear and Home
    }

    inline void home() {
        command(0x2);
        delayMicroseconds(1520);  // delay 1.52 ms between call for CLear and Home
    }
    inline void command(uint8_t value) {  // delay 37 us between each call (should be fine =P )
        send(value, LOW);
    }

    inline size_t write(uint8_t value) override {
        send(value, HIGH);
        return 1;
    }

   private:
    uint8_t _ser, _rclk, _srclk;
    inline void send(uint8_t value, uint8_t mode) {
        digitalWrite(_rclk, LOW);
        for (int8_t i = 7; i >= 0; i--) {
            digitalWrite(_ser, (value >> i) & 0x1);
            digitalWrite(_srclk, HIGH);
            digitalWrite(_srclk, LOW);
        }
        digitalWrite(_ser, mode);
        digitalWrite(_rclk, HIGH);
        digitalWrite(_rclk, LOW);
        digitalWrite(_ser, LOW);
    }
};

LCD1602REG lcd(11, 12, 13);


struct Menu {
    virtual void listen(InputEvent e);
    virtual void printTo(LCD1602REG &p);
    virtual void update(LCD1602REG &p);
};

namespace menu {
Menu *point[] = {};
uint8_t previous[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t pr_pos = 0;
uint8_t current = 0;

inline Menu *get_current() {
    return point[current];
}
inline void print_current() {
    get_current()->printTo(lcd);
}
void open(uint8_t to_open) {
    previous[++pr_pos] = to_open;
    current = to_open;
    print_current();
}
void close() {
    current = previous[--pr_pos];
    print_current();
}
};  // namespace menu

struct ListMenu : public Menu {
    char list[4][17] = {"^ Set Time ToDo ", "| Set Alarm Todo", "| Set Cond ToDo ", "# To Do         "};
    uint8_t i = 0;
    virtual void listen(InputEvent e) {
        switch (e.type) {
            case e.Press:
                menu::open(1);
                break;
            case e.Inc:
                if (i != 2) i += 1;
                break;
            case e.Dec:
                if (i != 0) i -= 1;
                break;
        }
    }
    virtual void printTo(LCD1602REG &p) {
        lcd.setCursor(0, 0);
        // lcd.clear();
    }
    virtual void update(LCD1602REG &p) {
        lcd.setCursor(0, 0);
        lcd.print(list[i]);
        lcd.setCursor(0, 1);
        lcd.print(list[i + 1]);
    }
};

struct SetTurnOn : public Menu {
    int state = 0;
    virtual void listen(InputEvent e) {
        switch (e.type) {
            case e.Press:
                // state++;
                // if (state == 4)
                menu::close();
                break;
            case e.Inc:
                break;
            case e.Dec:
                break;
        }
    }
    virtual void printTo(LCD1602REG &p) {
        p.setCursor(0, 0);
        p.print("Slct T to Arm Alarm");
        p.setCursor(0, 1);
        p.print("W:* H:**, M:** S:**");
    }
    virtual void update(LCD1602REG &p) {
        printTime(clock.t.time);
        // p.setCursor(2, 1);
        // p.print("");
    }
};

int DisplayTickFct(int state) {
    while (!event_queue.is_empty()) {
        dp_state.get_current()->listen(event_queue.pop());
    }
    dp_state.get_current()->update(lcd);
    return state;
}
}  // namespace display

namespace rotary_encoder {
void fall_clk();
void fall_dt();
void interRotEnc(uint8_t clk, uint8_t dt);
struct RotaryEncoder {
    RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw) : val(0), pin_clk(clk), pin_dt(dt), pin_sw(sw), pr_btn(0) {}

    void begin() {
        pinMode(pin_clk, INPUT);
        pinMode(pin_dt, INPUT);
        pinMode(pin_sw, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin_clk), fall_clk, FALLING);
        attachInterrupt(digitalPinToInterrupt(pin_dt), fall_dt, FALLING);
    }

    inline uint8_t read_clk() {
        return digitalRead(pin_clk);
    }

    inline uint8_t read_dt() {
        return digitalRead(pin_dt);
    }

    inline uint8_t read_sw() {
        uint8_t tmp = digitalRead(pin_sw);
        uint8_t res = pr_btn & !tmp;
        pr_btn = tmp;
        return res;
    }

    inline void add(int8_t dir) {
        val += dir;
    }

    int8_t clear() {
        noInterrupts();
        int8_t tmp = val;
        val = 0;
        interrupts();
        return tmp;
    }
    uint8_t pr_btn;
    uint8_t val;
    uint8_t pin_clk;
    uint8_t pin_dt;
    uint8_t pin_sw;
};

RotaryEncoder rot_enc(2, 3, 4);

void fall_clk() {
    interRotEnc(0, rot_enc.read_dt());
}

void fall_dt() {
    interRotEnc(rot_enc.read_clk(), 0);
}

inline void interRotEnc(uint8_t clk, uint8_t dt) {
    static short dir = 0;
    if (clk ^ dt) {
        dir = dt ? -1 : 1;
    } else if (!(clk & dt)) {
        rot_enc.add(dir);
        dir = 0;
    }
}

int RE_TickFct(int state) {
    int8_t tmp = rot_enc.clear();
    if (tmp > 0)
        event_queue.push({Event::Inc, (uint8_t)tmp});
    else if (tmp < 0)
        event_queue.push({Event::Dec, (uint8_t)-tmp});
    return state;
}

int RE_Btn_TickFct(int state) {
    uint8_t btn = rot_enc.read_sw();
    if (btn) event_queue.push({Event::Press, 1});
    return state;
}

}  // namespace rotary_encoder

namespace tasking {
struct Task {
    int state;
    uint16_t period;
    uint32_t lastRun;
    int (*TickFct)(int);
    void tick() {
        state = TickFct(state);
        lastRun = millis();
    }
};

Task tasks[] = {
        {0, 200, 0, rotary_encoder::RE_TickFct},
        {0, 50, 0, rotary_encoder::RE_Btn_TickFct},
        {0, 400, 0, display::DisplayTickFct},
        {0, 1000, 0, ClockTick}
    };

void tick_tasks() {
    for (auto &task : tasks)
        if (abs(millis() - task.lastRun) >= task.period) task.tick();
}
}  // namespace task

void setup() {
    Serial.begin(115200);
    display::Menu *p[2] = {new display::ListMenu(), new display::SetTurnOn()};
    rotary_encoder::rot_enc.begin();
    lcd.begin();
    clock.begin();
    usonic.begin();
}

void loop() {
    Serial.print("begin");
    tasking::tick_tasks();
}
