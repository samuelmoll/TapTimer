#ifndef VALVE_H
#define VALVE_H

#include "RTClib.h"
#include <stdint.h>

class Valve{
public:
    /**
     * Valve object to contain information about valve schedule information and hardware specifications
     *
     * @brief Object containing valve configuration data
     */
    Valve(int identifier, int pinNum);

    Valve() : Valve(0,0){};

    /**
     * Sets the Arduino numbered pin the valve is connected to
     * 
     * @brief Sets Arduino pin vlave is connected to.
     * 
     * @param Pin The Arduino numbered pin the valve is connected to. 
     */
    void setValvePin(int Pin);

    /**
     * Gets the Arduino numbered pin the valve is connected to
     * 
     * @brief Gets Arduino pin vlave is connected to.
     */
    int getValvePin();

    void setValveID(int ID); // currently unused

    int getValveID();  // currently unused

    /**
     * Toggles valve enabled state.
     * 
     * @brief Toggles valve enabled state.
     * 
     * @return The valve enabled state.
     */
    bool toggleValve();

    /**
     * Sets valve enabled state.
     * 
     * @brief Sets valve enabled state.
     * 
     * @param enable Boolean value for enable state.
     */
    void enableValve(bool enable);

    /**
     * Returns valve enabled state.
     * 
     * @brief Returns valve enabled state.
     * 
     * @return The valve enabled state.
     */
    bool isEnabled();

    /**
     * Sets valve start time (two int)
     * 
     * @brief Sets valve start time as two integers, hour and minute. This is saved to EEPROM
     * 
     * @param hour The start hour
     * @param minute The start minute
     */
    void setStartTime(int hour, int minute);

    /**
     * Returns start time as DateTime object
     * 
     * @brief Returns DateTime object
     * 
     * @return Start time as DateTime
     */
    DateTime getStartTime();

    int getStartHour();
    int getStartMinute();

    void setDuration(int periodInMinutes);

    int getDuration();

    /**
     * Calculates time valve turns off based upon start time and duration.
     * 
     * @brief Calculates time valve turns off based upon start time and duration, and returns as DateTime object
     * 
     * @return End time as DateTime object.
     */
    DateTime getEndTime();

    /**
     * Updates the valves start time based upon valve configuyration settings
     * 
     * @brief Updates internal valve start times based upon valve configuration for next start.
     */
    void updateNextStartTime();

    /**
     * Sets the periodicity of the valve - either as day intervals or as weekdays
     * 
     * @brief Sets the period - day interval or weekday.
     * 
     * @param period If MSB = 0, interpreted as day interval, else if MSB=1, interpreted as day flags (i.e *SMTWTFS)
     */
    void setPeriod(uint8_t period);

    uint8_t getPeriod();

    /**
     * Returns pointer to self
     * 
     * @brief Returns pointer to self (Valve object)
     * 
     * @return Pointer to self
     */
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