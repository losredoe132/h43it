// Define States 
State  SHORT ,   Long ,   Idle ,   IDLE ,   Short ,   LONG  ;

// Define transitions for each state

Transition SHORTTransitions[] = {
    
};
Transition LongTransitions[] = {
    
    { &IDLE, pred_button_released}  
    
};
Transition IdleTransitions[] = {
    
    { &SHORT, pred_button_short_push}  
    
};
Transition IDLETransitions[] = {
    
};
Transition ShortTransitions[] = {
    
    { &IDLE, pred_button_released} ,  
    
    { &LONG, pred_button_long_push}  
    
};
Transition LONGTransitions[] = {
    
};

// Define states with transitions

State SHORT = { "SHORT", SHORTAction, SHORTTransitions, 0 };

State Long = { "Long", LongAction, LongTransitions, 1 };

State Idle = { "Idle", IdleAction, IdleTransitions, 1 };

State IDLE = { "IDLE", IDLEAction, IDLETransitions, 0 };

State Short = { "Short", ShortAction, ShortTransitions, 2 };

State LONG = { "LONG", LONGAction, LONGTransitions, 0 };
