// Define States 
State  IDLE ,   SHORT ,   LONG  ;

// Define transitions for each state

Transition IDLETransitions[] = {
    
    { &SHORT, button_short_push }
    
};
Transition SHORTTransitions[] = {
    
    { &IDLE, button_released }
    
    { &LONG, button_long_push }
    
};
Transition LONGTransitions[] = {
    
    { &IDLE, button_released }
    
};

// Define states with transitions

State stateA = { "State A", stateAEnter, stateATransitions, 1 };

State stateA = { "State A", stateAEnter, stateATransitions, 1 };

State stateA = { "State A", stateAEnter, stateATransitions, 1 };
