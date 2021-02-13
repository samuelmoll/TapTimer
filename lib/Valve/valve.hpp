#ifndef VALVE_H
#define VALVE_H

#include "RTClib.h"
#include <stdint.h>

class Valve{
public:
    Valve(int identifier, int pinNum);

    Valve() : Valve(0,0){};

    void setValvePin(int Pin);

    int getValvePin();

    void setValveID(int ID);

    int getValveID();

    bool toggleValve();

    void enableValve(bool enable);

    bool isEnabled();

    void setStartTime(int hour, int minute);

    DateTime getStartTime();

    int getStartHour();
    int getStartMinute();

    void setDuration(int periodInMinutes);

    int getDuration();

    DateTime getEndTime();

    void updateNextStartTime();

    void setPeriod(uint8_t period);

    uint8_t getPeriod();

    Valve * returnSelf();

private:
    int id;
    int pin;
    bool valveEnabled;
    uint8_t startHour;
    uint8_t startMinute;
    uint8_t duration;
    uint8_t endHour;
    uint8_t endMinute;
    uint8_t period;
    /**
     * If bit 7 set, rest of bits represent days of week, starting from Sunday
     * Otherwise, number is days inbetween runs
    **/
};


#endif