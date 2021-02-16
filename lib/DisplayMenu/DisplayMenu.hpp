#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include "ValveController.hpp"
#include "LiquidCrystal_PCF8574.h"
#include "Keypad.h"
#include "RTClib.h"
#include <string.h>

class DisplayMenu{
    public:
        DisplayMenu(ValveController* VC, LiquidCrystal_PCF8574* LCD, Keypad* KP, RTC_DS3231* RTC);

        /**
         * Used to set valve period
         *
         * @brief Sets valve period, either by day interval or day of week.
         * 
         * @param Valve1 id The valve id, zero indexed
         */
        void setPeriod(int id);

        /**
         * Menu function for entering valve ID
         *
         * @brief Waits for user input of valve ID, and returns valve ID
         * 
         * @return Returns valve ID as int.
         */
        int enterValveID();

        /**
         * Menu function for entering valve start time
         *
         * @brief Waits for user input of start time as 4 digit sequence (24hr time), and sets valve object schedule.
         * 
         * @param valveID The valve ID, zero indexed
         */
        void setStartTime(int valveID);

        /**
         * Menu function for entering duration in minutes
         *
         * @brief Waits for user input of duration in minutes, ended by #, and sets valve object schedule.
         * 
         * @param valveID The valve ID, zero indexed
         */
        void setDuration(int valveID);

        /**
         * Menu function to get and display valve info on LCD screen.
         *
         * @brief Waits for user input of valve ID, then displays valve information.
         * 
         * @param valveID The valve ID, zero indexed
         */
        void displayValveInfo(int valveID);

        /**
         * Menu function to display initial options
         *
         * @brief Shows "main menu" (i.e. input options)
         */
        void displayOptions();

        /**
         * Menu function that waits for user input
         *
         * @brief Waits for unser input and returns keypad char.
         * 
         * @param scroll Determines whether LCD should scroll while awaiting input.
         * 
         * @return char: the ascii character pressed on the keypad.
         */
        char wait_for_keypad(bool scroll);

        /**
         * Menu function that immediately polls keypad.
         *
         * @brief Returns immediate keypress.
         * 
         * @return char: the ascii character pressed on the keypad.
         */
        char getKeypress();

        /**
         * Menu function to turn off LCD screen.
         *
         * @brief Turns off LCD.
         */
        void setLCDOff();

        /**
         * Main menu function to change behaviour based upon user input.
         *
         * @brief Main menu runtime, changing behaviour based upon user inputs.
         * 
         * @return bool: returns true when values have been saved to EEPROM.
         */
        int mainMenu();

        /**
         * Valve Coniguration menu for setting schedule information
         *
         * @brief Menu for configuring valve schedules
         * 
         * @return int: 
         */
        int valveConfigurationMenu();

        /**
         * Menu function to handle valve selection for a single run.
         *
         * @brief Handles valve selection for one-time run.
         * 
         * @return int: a set of flags for each valve. LSB is valve 1, MSB valve 8.
         */
        int valveSelectRunOnce();

        /**
         * Displays info while single run is operational
         *
         * @brief Displays info about which valve is running, and the time left.
         * 
         * @param valveInfo Pointer to valve information array furnished by the valve controller.
         */
        void valveRunOnceInfo(int valveInfo[]);

    private:
    ValveController* valveController;
    LiquidCrystal_PCF8574* lcd;
    Keypad* keypad;
    RTC_DS3231* rtc;
};


#endif