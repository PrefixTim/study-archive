#include <Print.h>
#include <Wire.h>
#include "task.h"

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

void setup() {
    Serial.begin(115200);
    tasking::force_tick_tasks();
    display::Menu *p[2] = {new display::ListMenu(), new display::SetTurnOn()};
    rotary_encoder::rot_enc.begin();
    lcd.begin();
    clock.begin();
    usonic.begin();
}

void loop() {
    tasking::tick_tasks();
}
