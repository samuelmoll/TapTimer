#include "EEPROMController.hpp"

EEPROMController::EEPROMController(Valve* v1, Valve* v2, Valve* v3, Valve* v4) {
  valve1 = v1;
  valve2 = v2;
  valve3 = v3;
  valve4 = v4;

  

}




void EEPROMController::initValves(){
  uint16_t sig;
  sig = eeprom_read_word(&signature);
  Serial.print(sig);
  if (sig == 0x74EE) { 
    Serial.print("EEPROM data detected - loading valves."); 
    uint8_t v1StartHour;
    uint8_t v1StartMin;
    uint8_t v1Duration;
    uint8_t v1Period;
    uint8_t v1Enable;
    v1StartHour = eeprom_read_byte(&valve1StartHour);
    v1StartMin = eeprom_read_byte(&valve1StartMin);
    v1Duration = eeprom_read_byte(&valve1Duration);
    v1Period = eeprom_read_byte(&valve1Period);
    v1Enable = eeprom_read_byte(&valve1Enable);
    valve1->setStartTime(v1StartHour, v1StartMin);
    valve1->setDuration(v1Duration);
    valve1->setPeriod(v1Period);
    valve1->enableValve(v1Enable);
    
    uint8_t v2StartHour;
    uint8_t v2StartMin;
    uint8_t v2Duration;
    uint8_t v2Period;
    uint8_t v2Enable;
    v2StartHour = eeprom_read_byte(&valve2StartHour);
    v2StartMin = eeprom_read_byte(&valve2StartMin);
    v2Duration = eeprom_read_byte(&valve2Duration);
    v2Period = eeprom_read_byte(&valve2Period);
    v2Enable = eeprom_read_byte(&valve2Enable);
    valve2->setStartTime(v2StartHour, v2StartMin);
    valve2->setDuration(v2Duration);
    valve2->setPeriod(v2Period);
    valve2->enableValve(v2Enable);

    uint8_t v3StartHour;
    uint8_t v3StartMin;
    uint8_t v3Duration;
    uint8_t v3Period;
    uint8_t v3Enable;
    v3StartHour = eeprom_read_byte(&valve3StartHour);
    v3StartMin = eeprom_read_byte(&valve3StartMin);
    v3Duration = eeprom_read_byte(&valve3Duration);
    v3Period = eeprom_read_byte(&valve3Period);
    v3Enable = eeprom_read_byte(&valve3Enable);
    valve3->setStartTime(v3StartHour, v3StartMin);
    valve3->setDuration(v3Duration);
    valve3->setPeriod(v3Period);
    valve3->enableValve(v3Enable);

    uint8_t v4StartHour;
    uint8_t v4StartMin;
    uint8_t v4Duration;
    uint8_t v4Period;
    uint8_t v4Enable;
    v4StartHour = eeprom_read_byte(&valve4StartHour);
    v4StartMin = eeprom_read_byte(&valve4StartMin);
    v4Duration = eeprom_read_byte(&valve4Duration);
    v4Period = eeprom_read_byte(&valve4Period);
    v4Enable = eeprom_read_byte(&valve4Enable);
    valve4->setStartTime(v4StartHour, v4StartMin);
    valve4->setDuration(v4Duration);
    valve4->setPeriod(v4Period);
    valve4->enableValve(v4Enable);
  }
}

void EEPROMController::updateEEPROM(){
    Serial.println("updating EEprom");

    uint8_t v1StartHour = valve1->getStartHour();
    uint8_t v1StartMin = valve1->getStartMinute();
    uint8_t v1Duration = valve1->getDuration();
    uint8_t v1Period = valve1->getPeriod();
    uint8_t v1Enable = valve1->isEnabled();
    eeprom_update_byte((uint8_t*)&valve1StartHour, v1StartHour);
    eeprom_update_byte((uint8_t*)&valve1StartMin, v1StartMin);
    eeprom_update_byte((uint8_t*)&valve1Duration, v1Duration);
    eeprom_update_byte((uint8_t*)&valve1Period, v1Period);
    eeprom_update_byte((uint8_t*)&valve1Enable, v1Enable);
    
    uint8_t v2StartHour = valve2->getStartHour();
    uint8_t v2StartMin = valve2->getStartMinute();
    uint8_t v2Duration = valve2->getDuration();
    uint8_t v2Period = valve2->getPeriod();
    uint8_t v2Enable = valve2->isEnabled();
    eeprom_update_byte((uint8_t*)&valve2StartHour, v2StartHour);
    eeprom_update_byte((uint8_t*)&valve2StartMin, v2StartMin);
    eeprom_update_byte((uint8_t*)&valve2Duration, v2Duration);
    eeprom_update_byte((uint8_t*)&valve2Period, v2Period);
    eeprom_update_byte((uint8_t*)&valve2Enable, v2Enable);

    uint8_t v3StartHour = valve3->getStartHour();
    uint8_t v3StartMin = valve3->getStartMinute();
    uint8_t v3Duration = valve3->getDuration();
    uint8_t v3Period = valve3->getPeriod();
    uint8_t v3Enable = valve3->isEnabled();
    eeprom_update_byte((uint8_t*)&valve3StartHour, v3StartHour);
    eeprom_update_byte((uint8_t*)&valve3StartMin, v3StartMin);
    eeprom_update_byte((uint8_t*)&valve3Duration, v3Duration);
    eeprom_update_byte((uint8_t*)&valve3Period, v3Period);
    eeprom_update_byte((uint8_t*)&valve3Enable, v3Enable);

    uint8_t v4StartHour = valve4->getStartHour();
    uint8_t v4StartMin = valve4->getStartMinute();
    uint8_t v4Duration = valve4->getDuration();
    uint8_t v4Period = valve4->getPeriod();
    uint8_t v4Enable = valve4->isEnabled();
    eeprom_update_byte((uint8_t*)&valve4StartHour, v4StartHour);
    eeprom_update_byte((uint8_t*)&valve4StartMin, v4StartMin);
    eeprom_update_byte((uint8_t*)&valve4Duration, v4Duration);
    eeprom_update_byte((uint8_t*)&valve4Period, v4Period);
    eeprom_update_byte((uint8_t*)&valve4Enable, v4Enable);

    eeprom_update_word((uint16_t*)&signature, 0x74EE);
}