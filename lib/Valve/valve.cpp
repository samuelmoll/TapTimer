#include "valve.hpp"

Valve::Valve(int identifier, int pinNum) {
    id = identifier;
    pin = pinNum;
}



void Valve::setValvePin(int Pin) {
    pin = Pin;
}

int Valve::getValvePin(){
    return pin;
}

void Valve::setValveID(int ID){
    id = ID;
}

int Valve::getValveID(){
    return id;
}

bool Valve::toggleValve(){
    valveEnabled = !valveEnabled;
    return valveEnabled;
}

void Valve::enableValve(bool enable){
    valveEnabled = enable;
}

bool Valve::isEnabled(){
    return valveEnabled;
}


int Valve::getStartHour(){
     return startHour;
}

int Valve::getStartMinute(){
    return startMinute;
}

void Valve::setStartTime(int hour, int min) {
    startHour = hour;
    startMinute = min;
}

int Valve::getDuration(){
    return duration;
}

void Valve::setDuration(int Duration) {
    duration = Duration;
}

uint8_t Valve::getPeriod(){
    return period;
}
void Valve::setPeriod(uint8_t Period){
    period = Period;
}

Valve * Valve::returnSelf(){
    return this;
}