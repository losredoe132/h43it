// Define States 
State  Long ,   Idle ,   Short  ;

// Define transitions for each state

Transition LongTransitions[] = {
    
    { &Idle, pred_button_released}  
    
};
Transition IdleTransitions[] = {
    
    { &Short, pred_button_short_push}  
    
};
Transition ShortTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Long, pred_button_long_push}  
    
};

// Define states with transitions

State Long = { "Long", LongAction, LongTransitions, 1 };

State Idle = { "Idle", IdleAction, IdleTransitions, 1 };

State Short = { "Short", ShortAction, ShortTransitions, 2 };
