#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include "defines.hpp"

#include "DisplayMenu.hpp"
#include "ValveController.hpp"
#include "EEPROMController.hpp"

#include "States.hpp"

class AbstractState;

/**
 * @brief Finite State Machine
 * 
 * Expose a single update function to transition between states.
 */
class StateMachine{
    friend class AbstractState;
    public:
        /// Constructor
        StateMachine(DisplayMenu* menu, ValveController* valveControl, EEPROMController* memControl, AbstractState* startState, bool enableTransitions=true):
                      displayMenu(menu),
                      valveController(valveControl),
                      eepromController(memControl),
                      state(startState),
                      transition(enableTransitions){
                    }

        /**
         * transition Get Method
         *
         * @brief Get whether states should transition or not.
         * 
         * @return Whether transitions are enabled.
         */
        bool transitionsEnabled(){return transition;}

        /**
         * Find the angle correspdoning to the minimum ultrasonic reading.
         *
         * @brief Run the update loop corresponding to the current state.
         */
        void update();

        /**
         * localiser Get Method
         *
         * @brief Get the pointer the localiser.
         * 
         * @return The localiser pointer.
         */
        DisplayMenu* getDisplayMenu(){return displayMenu;};

        /**
         * targeting Get Method
         *
         * @brief Get the pointer to the targeter
         * 
         * @return The targeting pointer.
         */
        ValveController* getValveController(){return valveController;};

        /**
         * targeting Get Method
         *
         * @brief Get the pointer to the targeter
         * 
         * @return The targeting pointer.
         */
        EEPROMController* getEEPROMController(){return eepromController;};



        /// Deconstructor
        ~StateMachine(){delete state;}

      private:

        /// State Variables
        AbstractState* state;
        bool transition;

        DisplayMenu* displayMenu;
        ValveController* valveController;
        EEPROMController* eepromController;
};


#endif