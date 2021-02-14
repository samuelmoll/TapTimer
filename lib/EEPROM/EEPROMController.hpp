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

    private:
        uint16_t EEMEM signature;

        uint8_t EEMEM valve1StartHour;
        uint8_t EEMEM valve1StartMin;
        uint8_t EEMEM valve1Duration;
        uint8_t EEMEM valve1Period;
        uint8_t EEMEM valve1Enable;

        uint8_t EEMEM valve2StartHour;
        uint8_t EEMEM valve2StartMin;
        uint8_t EEMEM valve2Duration;
        uint8_t EEMEM valve2Period;
        uint8_t EEMEM valve2Enable;

        uint8_t EEMEM valve3StartHour;
        uint8_t EEMEM valve3StartMin;
        uint8_t EEMEM valve3Duration;
        uint8_t EEMEM valve3Period;
        uint8_t EEMEM valve3Enable;

        uint8_t EEMEM valve4StartHour;
        uint8_t EEMEM valve4StartMin;
        uint8_t EEMEM valve4Duration;
        uint8_t EEMEM valve4Period;
        uint8_t EEMEM valve4Enable;

        Valve* valve1;
        Valve* valve2;
        Valve* valve3;
        Valve* valve4;
};

#endif