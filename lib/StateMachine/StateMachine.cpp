#include "StateMachine.hpp"

void StateMachine::update(){state->update(*this);}
