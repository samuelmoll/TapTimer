#ifndef EEPROMCONTROLLER_H
#define EEPROMCONTROLLER_H

#include <stdint.h>
#include <avr/eeprom.h>
#include <Arduino.h>

#include "valve.hpp"



class EEPROMController{
    public:
        EEPROMController(Valve* v1, Valve* v2, Valve* v3, Valve* v4);
        
        void initValves();

        void updateEEPROM();

        static uint16_t signature;

        static uint8_t  valve1StartHour;
        static uint8_t  valve1StartMin;
        static uint8_t  valve1Duration;
        static uint8_t  valve1Period;
        static uint8_t  valve1Enable;

        static uint8_t  valve2StartHour;
        static uint8_t  valve2StartMin;
        static uint8_t  valve2Duration;
        static uint8_t  valve2Period;
        static uint8_t  valve2Enable;

        static uint8_t  valve3StartHour;
        static uint8_t  valve3StartMin;
        static uint8_t  valve3Duration;
        static uint8_t  valve3Period;
        static uint8_t  valve3Enable;

        static uint8_t  valve4StartHour;
        static uint8_t  valve4StartMin;
        static uint8_t  valve4Duration;
        static uint8_t  valve4Period;
        static uint8_t  valve4Enable;

    private:
        Valve* valve1;
        Valve* valve2;
        Valve* valve3;
        Valve* valve4;
};

#endif