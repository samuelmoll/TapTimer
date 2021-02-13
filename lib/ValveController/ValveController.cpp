#include "ValveController.hpp"


ValveController::ValveController(Valve* Valve1, Valve* Valve2, Valve* Valve3, Valve* Valve4, RTC_DS3231* RTC){
    valves[0] = Valve1;
    valves[1] = Valve2;
    valves[2] = Valve3;
    valves[3] = Valve4;
    rtc = RTC;
}

Valve** ValveController::getValves(){
    return valves;
}

int ValveController::getNextWeekdayInterval(int valveID, DateTime now) {
    uint8_t period = valves[valveID]->getPeriod();
    int dayInterval = 0;
    for(int d = 0; d <7; d++) {
        if(period & (1<<(6-d))) {
            dayInterval = d - startDateTimes[valveID].dayOfTheWeek();
            Serial.println(dayInterval);
            if(dayInterval > 0) {return dayInterval;}
            else if (dayInterval == 0) {
                if(startDateTimes[valveID].hour() > now.hour()) {return dayInterval;}
            }
        }
    }
    // Gets here: all days are earlier in the week, so find interval by adding 7 to day. THe first day found will be the next day.
    for(int d = 0; d <7; d++) {
        if(period & (1<<(6-d))) {
            dayInterval = d + 7 - startDateTimes[valveID].dayOfTheWeek();
            return dayInterval;
        }
    }
    // should never get here without dayInterval being correct
    Serial.print("Final day interval (getNextWeekdayInterval): "); Serial.println(dayInterval);
    startDateTimes[valveID] = startDateTimes[valveID] + TimeSpan(dayInterval,0,0,0);
    return dayInterval;
}

void ValveController::initSchedule() {
    DateTime now = rtc->now();
    for(int i = 0; i < NUM_VALVES; i++) {
        startDateTimes[i] = DateTime(now.year(),now.month(), now.day(), (int)valves[i]->getStartHour(), (int)valves[i]->getStartMinute(), 0);
        uint8_t period = valves[i]->getPeriod();
        int dayInterval = 0;;
        if (period & 0x80) {
            dayInterval = getNextWeekdayInterval(i, now);
        } else {
            if (startDateTimes[i].hour() < now.hour() || 
                    (startDateTimes[i].hour() == now.hour() && startDateTimes[i].minute() < now.minute())) {
                dayInterval = 1;
            }
        }
        startDateTimes[i] = startDateTimes[i] + TimeSpan(dayInterval,0,0,0);
        char s[18] = {};
        sprintf(s, "V%d: %2d-%2d %d:%02d",  i,
                                            startDateTimes[i].day(),
                                            startDateTimes[i].month(),
                                            startDateTimes[i].hour(),
                                            startDateTimes[i].minute());
        Serial.print("Next start: "); Serial.println(s);
    }
}

void ValveController::runSchedule(){
    DateTime now = rtc->now();
    for(int i = 0; i < NUM_VALVES; i++) {
        if (valves[i]->isEnabled()) {
            DateTime endTime = startDateTimes[i] + TimeSpan(0,0,valves[i]->getDuration(), 0);
            //Serial.print(endTime.hour()); Serial.println(endTime.minute()); //check
            if (startDateTimes[i].unixtime() < now.unixtime()
                                    && endTime.unixtime() > now.unixtime()) {
                digitalWrite(valvePins[i], HIGH);
                checkValves |= (1<<i);
            } else {
                digitalWrite(valvePins[i], LOW);
                if (endTime.unixtime() < now.unixtime()) {  // Update next run time for this valve. Intended to occur first time endTime is in the past.
                    uint8_t period = valves[i]->getPeriod();
                    int dayInterval = 0;
                    if (period & 0x80) { // Weekday setting
                        dayInterval = getNextWeekdayInterval(i, now);
                        startDateTimes[i] = startDateTimes[i] +TimeSpan(dayInterval,0,0,0);
                    } else {
                        startDateTimes[i] = startDateTimes[i] + TimeSpan(period,0,0,0);
                    }
                }
            }
        }
    }
}

void ValveController::runScheduleOnce(int valveSelect) {
    DateTime now = rtc->now();
    TimeSpan durationNow = TimeSpan(0,0,0,0);
    DateTime startTimeNow[NUM_VALVES] = {};
    DateTime endTimeNow[NUM_VALVES] = {};
    int last_valve = 0;
    for(int i = 0; i < NUM_VALVES; i++) {
        if (valveSelect & (1<<i)) {
            last_valve = i;
            startTimeNow[i] = now + durationNow;
            durationNow = TimeSpan(0,0,valves[i]->getDuration(), 0) + durationNow;
            endTimeNow[i] = startTimeNow[i] + TimeSpan(0,0,valves[i]->getDuration(), 0);
            // check minutes are right
            Serial.print(i+1); Serial.print(": ");
            Serial.print(startTimeNow[i].hour()); Serial.print(startTimeNow[i].minute());
            Serial.print(":");
            Serial.print(endTimeNow[i].hour()); Serial.println(endTimeNow[i].minute());
        }
    }
    bool run = 1;
    while(run) {
        Serial.print(run);
        now = rtc->now();
        for(int i = 0; i < NUM_VALVES; i++) {
            if (startTimeNow[i].unixtime() < now.unixtime()
                                    && endTimeNow[i].unixtime() > now.unixtime()) {
                digitalWrite(valvePins[i], HIGH);
            } else {
                digitalWrite(valvePins[i], LOW);
            }
        }
        if (endTimeNow[last_valve].unixtime() < now.unixtime()) {
            run = 0;
        }
        delay(1000);
    }
}