#ifndef STATES_H
#define STATES_H

#include <Arduino.h>
#include "defines.hpp"
#include "ValveController.hpp"
#include "DisplayMenu.hpp"
#include "EEPROMController.hpp"

#include "StateMachine.hpp"

class StateMachine;

class AbstractState{
    public:
        /**
         * State Update Loop
         *
         * @brief No actual functionality only placeholder for child classes.
         * 
         * @param machine The controlling state machine.
         */
        virtual void update(StateMachine& machine) = 0;

        /// Deconstructor
        virtual ~AbstractState(){};

    protected:
        /**
         * Move between states
         *
         * @brief Update the state machine to the given state.
         * 
         * @param machine The controlling state machine.
         * @param st The state to transition to.
         */
        void setState(StateMachine& machine, AbstractState* st);
};

/* STATES */

class InitScheduleState : public AbstractState {
    public:
        /**
         * State Update Loop
         *
         * @brief Perform horizontal targeting.
         * 
         * @param machine The controlling state machine.
         */
        virtual void update(StateMachine& machine);

        /// Deconstructor
        virtual ~InitScheduleState(){};
};

class RunScheduleState : public AbstractState {
    public:
        /**
         * State Update Loop
         *
         * @brief Perform horizontal targeting.
         * 
         * @param machine The controlling state machine.
         */
        virtual void update(StateMachine& machine);

        /// Deconstructor
        virtual ~RunScheduleState(){};
};

class MainMenuState : public AbstractState {
    public:
        /**
         * State Update Loop
         *
         * @brief Contains Main Menu state instructions
         * 
         * @param machine The controlling state machine.
         */
        virtual void update(StateMachine& machine);

        /// Deconstructor
        virtual ~MainMenuState(){};
};

class RunOnceState : public AbstractState {
    public:
        /**
         * State Update Loop
         *
         * @brief Enters single run state setup and execution.
         * 
         * @param machine The controlling state machine.
         */
        virtual void update(StateMachine& machine);

        /// Deconstructor
        virtual ~RunOnceState(){};
};




#endif