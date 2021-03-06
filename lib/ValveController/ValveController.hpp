#ifndef VALVECONTROLLER_H
#define VALVECONTROLLER_H

#include "valve.hpp"
#include "defines.hpp"
using namespace std;

/**
     * Object to hold Valve objects and run schedule
     *
     * @brief COntains valve control/scheduling functions
     * 
     * @param Valve1 Valve 1
     * @param Valve2 Valve 2
     * @param Valve3 Valve 3
     * @param Valve4 Valve 4
     * @param RTC Real-Time clock object
     */
class ValveController{
    public:

    /**
     * Object to hold Valve objects and run schedule
     *
     * @brief COntains valve control/scheduling functions
     * 
     * @param Valve1 Valve 1
     * @param Valve2 Valve 2
     * @param Valve3 Valve 3
     * @param Valve4 Valve 4
     * @param RTC Real-Time clock object
     */
    ValveController(Valve* Valve1, Valve* Valve2, Valve* Valve3, Valve* Valve4, RTC_DS3231* RTC);

    /**
     * Turns all valves off (i.e. sends a low signal)
     *
     * @brief Writes a zero to all valve relays.
     */
    void setAllValvesOff();

    /**
     * Returns the Valve pointer array
     *
     * @brief Returns the Valve object pointer array
     *      * 
     * @return Returns Valve Objects pointer array
     */
    Valve** getValves();

    /**
     * Returns the boolean flags indicating whether a valve is on.
     *
     * @brief Returns th boolean flags indicating a vavle is on (zero-indexed)
     * 
     * @return int: 8 bit flag (LSB is Valve "1", valve ID 0)
     */
    int getCheckValves();

    /**
     * Returns currently running valve info for menu display.
     *
     * @brief Returns valve ID of currently running valve, and time in mins until valve turns off.
     * 
     * @return int[2]: contains valve ON [0], and time left[1].
     */
    int * getValveInfoRunOnce();

    int getStartTimeHourRunOnce(int valveID);
    int getStartTimeMinRunOnce(int valveID);
    int getEndTimeHourRunOnce(int valveID);
    int getEndTimeMinRunOnce(int valveID);
    int getValveDuration(int valveID);

    /**
     * Returns the number of days until the next schedulaed start
     *
     * @brief Returns number of days until next scheduled start
     * 
     * @param valveID The valve ID number (Zero indexed)
     * @param now A DateTime object with the current time
     * 
     * @return int: number of days until next scheduled start
     */
    int getNextWeekdayInterval(int valveID, DateTime now);

    /**
     * Generates the DateTime objects for the scheduled start anf end times of valves.
     *
     * @brief Initialises the schedule
     * 
     */
    void initSchedule();

    /**
     * Runs the schedule by checking whether current time is between start and stop times, and 
     * sending HIGH or LOW signal to valve.
     *
     * @brief Runs the schedule. Must be called iteratively.
     */
    void runSchedule();

    /**
     * Generates a schedule to start immediately, using the duration as intervals between start and end times 
     *
     * @brief Initialises schedule for single run
     */
    int runScheduleOnceInit(int valveSelect);

    /**
     * Runs consecutive stations once. Ignores day interval settings. 
     *
     * @brief Runs each station consecutively once, immediately
     */
    int runScheduleOnce(int valveSelect, int last_valve);

    private:
    Valve* valves[NUM_VALVES];                                          // Array holding valve objects belonging to controller
    RTC_DS3231* rtc;                                                    // Real-Time Clock object 
    uint8_t valvePins[NUM_VALVES] = {RELAY1, RELAY2, RELAY3, RELAY4};   // Array storing the pins used to control valves
    DateTime startDateTimes[4];                                         // Stores start times for building schedule
    int checkValves = 0x00;                                             // Stores flags for valves on or off
    DateTime startTimeNow[NUM_VALVES];
    DateTime endTimeNow[NUM_VALVES];
};

#endif