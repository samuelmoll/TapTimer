#include "States.hpp"

void AbstractState::setState(StateMachine& machine, AbstractState* state) {
    AbstractState* aux = machine.state;
    machine.state = state; 
    delete aux;
}

/**
 * ========================================
 *           States
 * ========================================
 */

void RunScheduleState::update(StateMachine& machine){
  Serial.println("Running Schedule:");
  char keypressed = NO_KEY;
  while(keypressed == NO_KEY) {
      machine.getValveController()->runSchedule();
      keypressed = machine.getDisplayMenu()->getKeypress();
  }
  if(machine.transitionsEnabled()){setState(machine, new MainMenuState());}
}


void MainMenuState::update(StateMachine& machine){
  Serial.println("Main Menu State:");
  char nextState;
  nextState = machine.getDisplayMenu()->mainMenu();
  if(nextState == 1 || nextState == 0) {
    if (nextState == 1) {machine.getEEPROMController()->updateEEPROM();}
    if(machine.transitionsEnabled()) {setState(machine, new RunScheduleState());}
  } else if(nextState == 2) {
    if(machine.transitionsEnabled()) {setState(machine, new RunOnceState());}
  }
}

void RunOnceState::update(StateMachine& machine){
  Serial.println("Run Once State:");
  int valveSelect = machine.getDisplayMenu()->valveSelectRunOnce();
  if(valveSelect <= 0) {
    machine.getDisplayMenu()->setLCDOff();
    if(machine.transitionsEnabled()) {setState(machine, new RunScheduleState());}
  } else {
    int last_valve = machine.getValveController()->runScheduleOnceInit(valveSelect);
    int run = 1;
    int lastValveState = 0;
    int currentValveState = 0;
    machine.getDisplayMenu()->setLCDOff();
    while(run && (machine.getDisplayMenu()->getKeypress() != '*')) {
      run = machine.getValveController()->runScheduleOnce(valveSelect, last_valve);
      currentValveState = machine.getValveController()->getCheckValves();
      if(currentValveState != lastValveState) {
        machine.getDisplayMenu()->valveRunOnceInfo(currentValveState);
      }
    } 
  }
  machine.getValveController()->setAllValvesOff();
  machine.getValveController()->initSchedule();
  if(machine.transitionsEnabled()) {setState(machine, new RunScheduleState());}
}