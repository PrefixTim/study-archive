#include "tmalk001_alarm_sch.h"
#include "tmalk001_dsclock.h"
#include "tmalk001_glue.h"

namespace alarm {
    int AlarmSchTickFct(int state) {
        static uint8_t i = 0;
        switch (state) {
        case Al_Sch_Start:
            state = Al_Sch_Run;
            break;

        case Al_Sch_Run:
            i = mtime::clock.t.time.minutes;
            if (glue::schedule.turn_on_schedule[mtime::clock.t.time.week_day - 1].hours == mtime::clock.t.time.hours && glue::schedule.turn_on_schedule[mtime::clock.t.time.week_day - 1].minutes == mtime::clock.t.time.minutes) {
                glue::lock_req_queue.push({glue::LockReq::Lock});
                state = Al_Sch_Wait;
            }
            if (glue::schedule.turn_off_schedule[mtime::clock.t.time.week_day - 1].hours == mtime::clock.t.time.hours && glue::schedule.turn_off_schedule[mtime::clock.t.time.week_day - 1].minutes == mtime::clock.t.time.minutes) {
                glue::lock_req_queue.push({glue::LockReq::Unlock});
                state = Al_Sch_Wait;
            }
            break;
        case Al_Sch_Wait:
            if (i != mtime::clock.t.time.minutes) {
                    state = Al_Sch_Run;
                }
            break;
        }
        return state;
    }
} // namespace alarm

namespace display {
   
        void AlarmSetMenu::listen(glue::InputEvent e) {
        if (e.type == e.Press) {
            if (i < 3) {
                i++;
            } else {
                if (i == 3) {
                    glue::schedule.turn_off_schedule[w] = {128, mtime::dec2bcd(m), mtime::dec2bcd(h), 128, 128, 128, w};
                } else if (i == 4) {
                    glue::schedule.turn_on_schedule[w] = {128, mtime::dec2bcd(m), mtime::dec2bcd(h), 128, 128, 128, w};
                }
                    menu.close(2);
            }
        } else if(e.type == e.Inc || e.type == e.Dec) {
            if (i == 0) {
                w += e.type == e.Inc ? e.val : -e.val;
                if (w > 100)
                    w = 6;
                w %= 7;
            } else  if (i == 1) {
                h += e.type == e.Inc ? e.val : -e.val;
                if (h > 100)
                    h = 24;
                h %= 25;
            } else if (i == 2) {
                m += e.type == e.Inc ? e.val : -e.val;
                if (m > 100)
                    m = 59;
                m %= 60;
            } else {
                if (e.type == e.Inc) {
                    i = 4;
                } else {
                    i = 3;
                }
            }
        }
    }
    void AlarmSetMenu::start() {
        w = 0;
        h = 0;
        m = 0;
        i = 0;
        lcd.setCursor(0, 0);
        lcd.print("Chose time    ");
        lcd.setCursor(0, 1);
        lcd.print("D:*** H:** M:**");
    }

    void AlarmSetMenu::update() {
        if (i < 3) {
            lcd.setCursor(2, 1);
            lcd.print(mtime::TimeBcd::week_days[w+1]);
            lcd.setCursor(8, 1);
            lcd.print(h / 10);
            lcd.print(h % 10);
            lcd.setCursor(13, 1);
            lcd.print(m / 10);
            lcd.print(m % 10);
        } else {
            lcd.setCursor(0, 0);
            lcd.print("State         ");
            lcd.setCursor(0, 1);
            lcd.print(i == 3 ? "<Off>  On      " : " Off  <On>     ");
        }
    }
} // namespace display